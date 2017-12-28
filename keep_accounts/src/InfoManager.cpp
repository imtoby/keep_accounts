#include "InfoManager.h"
#include "DBManager.h"
#include "TypeInfo.h"
#include "ZUuid.h"
#include "base/kglobal.h"

#include <QDebug>
#include <QDateTime>
#include <QThread>
#include <QStringList>

namespace {

TypeModel* TopInModel       = NULL;
TypeModel* ChildInModel     = NULL;
TypeModel* TopOutModel      = NULL;
TypeModel* ChildOutModel    = NULL;

RecordModel* CurrentRecordModel = NULL;

int GetCount(int type, const QString &parentId)
{
    if (KA::OUT == type) {
        if (parentId == KA::TOP_TYPE_ID) {
            return TopOutModel->count();
        } else {
            return ChildOutModel->count();
        }
    } else {
        if (parentId == KA::TOP_TYPE_ID) {
            return TopInModel->count();
        } else {
            return ChildInModel->count();
        }
    }
}

bool INIT_TYPE_INFO_FINISHED    = false;
bool INIT_RECORD_DATA_FINISHED  = false;

}

class InfoManagerPrivate
{
public:
    InfoManagerPrivate(InfoManager *parent)
        : q_ptr(parent)
        , worker(NULL)
        , selectInTypeModel(NULL)
        , selectOutTypeModel(NULL)
    {}

    void init();
    void uninit();

private:
    InfoManager * const q_ptr;
    Q_DECLARE_PUBLIC(InfoManager)

    QThread workerThread;
    Worker *worker;
    TypeModel* selectInTypeModel;
    TypeModel* selectOutTypeModel;
};

InfoManager::InfoManager(QObject *parent)
    : QObject(parent)
    , d_ptr(new InfoManagerPrivate(this))
{
    Q_D(InfoManager);
    d->init();

    d->worker->moveToThread(&d->workerThread);
    connect(&d->workerThread, &QThread::finished, d->worker,
            &QObject::deleteLater);
    connect(this, &InfoManager::addType, this, &InfoManager::doAddType);
    connect(d->worker, &Worker::addType, d->worker, &Worker::doAddType);
    connect(d->worker, &Worker::addTypeFinished, this,
            &InfoManager::doAddTypeFinished);

    connect(this, &InfoManager::deleteType, d->worker, &Worker::doDeleteType);
    connect(d->worker, &Worker::deleteTypeFinished,
            this, &InfoManager::doDeleteTypeFinished);

    connect(this, &InfoManager::addRecord, this, &InfoManager::doAddRecord);
    connect(d->worker, &Worker::addRecord, d->worker, &Worker::doAddRecord);
    connect(d->worker, &Worker::addRecordFinished, this,
            &InfoManager::doAddRecordFinished);

    connect(this, &InfoManager::deleteRecord, d->worker, &Worker::doDeleteRecord);
    connect(d->worker, &Worker::deleteRecordFinished,
            this, &InfoManager::doDeleteRecordFinished);

    connect(this, &InfoManager::refreshSelectTypeData, this,
            &InfoManager::initSelectTypeData);

    connect(this, &InfoManager::updateRecord, this,
            &InfoManager::doUpdateRecord);
    connect(d->worker, &Worker::updateRecord, d->worker,
            &Worker::doUpdateRecord);
    connect(d->worker, &Worker::updateRecordFinished, this,
            &InfoManager::doUpdateRecordFinished);

    d->workerThread.start();
}

InfoManager::~InfoManager()
{
    Q_D(InfoManager);
    d->uninit();
    d->workerThread.quit();
    d->workerThread.wait();
}

void InfoManager::initData()
{
    initTypeData();
    initRecordData();
}

TypeModel *InfoManager::editTypeModel(int type, const QString& parentId)
{
    if (KA::OUT == type) {
        if (parentId == KA::TOP_TYPE_ID) {
            return TopOutModel;
        } else {
            QObjectList infos = KA_DB->getTypeInfos(KA::OUT, this, parentId);
            ChildOutModel->clear();
            ChildOutModel->set(&infos);
            return ChildOutModel;
        }
    } else {
        if (parentId == KA::TOP_TYPE_ID) {
            return TopInModel;
        } else {
            QObjectList infos = KA_DB->getTypeInfos(KA::IN, this, parentId);
            ChildInModel->clear();
            ChildInModel->set(&infos);
            return ChildInModel;
        }
    }

    return TopOutModel;
}

TypeModel *InfoManager::selectTypeModel(int type)
{
    Q_D(InfoManager);
    if (KA::OUT == type) {
        return d->selectOutTypeModel;
    } else {
        return d->selectInTypeModel;
    }
}

void InfoManager::setTypeName(int type, const QString &typeId,
                              const QString &parentId,
                              const QString &typeName)
{
    bool success = KA_DB->updateTypeInfo(typeId, KA::TYPE_NAME, typeName);
    if (success) {
        TypeInfo* info = NULL;
        if (KA::OUT == type) {
            if (parentId == KA::TOP_TYPE_ID) {
                info = TopOutModel->getTypeInfoByTypeId(typeId);
            } else {
                info = ChildOutModel->getTypeInfoByTypeId(typeId);
            }
        } else {
            if (parentId == KA::TOP_TYPE_ID) {
                info = TopInModel->getTypeInfoByTypeId(typeId);
            } else {
                info = ChildInModel->getTypeInfoByTypeId(typeId);
            }
        }
        if (NULL != info) {
            info->setTypeName(typeName);
        }
    } else {
        qDebug() << __FUNCTION__ << "doSetTypeName failed";
    }
}

RecordModel *InfoManager::recordModel(int year, int month)
{
    if (-1 == year && -1 == month) {
        return CurrentRecordModel;
    }

    QObjectList datas = KA_DB->getRecordItems(year, month, this);
    CurrentRecordModel->clear();
    CurrentRecordModel->set(&datas);
    return CurrentRecordModel;
}

QString InfoManager::getTypeName(const QString &typeId) const
{
    return KA_DB->getTypeName(typeId);
}

QString InfoManager::getParentTypeName(int type, const QString &parentId) const
{
    TypeInfo* info = NULL;
    if (KA::OUT == type) {
        info = TopOutModel->getTypeInfoByTypeId(parentId);
    } else {
        info = TopInModel->getTypeInfoByTypeId(parentId);
    }
    if (NULL != info) {
        return info->typeName();
    }
    return QString();
}

void InfoManager::initTypeData()
{
    if (!INIT_TYPE_INFO_FINISHED) {
        QObjectList infos = KA_DB->getTypeInfos(KA::IN, this, KA::TOP_TYPE_ID);
        TopInModel->clear();
        TopInModel->set(&infos);

        infos = KA_DB->getTypeInfos(KA::OUT, this, KA::TOP_TYPE_ID);
        TopOutModel->clear();
        TopOutModel->set(&infos);

        initSelectTypeData();

        INIT_TYPE_INFO_FINISHED = true;
    }

    emit initTypeFinished();
}

void InfoManager::initSelectTypeData()
{
    Q_D(InfoManager);

    d->selectOutTypeModel->clear();
    for (int i=0; i<TopOutModel->count(); ++i) {
        TypeInfo* typeInfo = qobject_cast<TypeInfo*>(TopOutModel->get(i));
        d->selectOutTypeModel->append(typeInfo);
        QObjectList infos = KA_DB->getTypeInfos(KA::OUT, this,
                                                typeInfo->typeId());
        d->selectOutTypeModel->appendList(&infos);
    }

    d->selectInTypeModel->clear();
    for (int i=0; i<TopInModel->count(); ++i) {
        TypeInfo* typeInfo = qobject_cast<TypeInfo*>(TopInModel->get(i));
        d->selectInTypeModel->append(typeInfo);
        QObjectList infos = KA_DB->getTypeInfos(KA::IN, this,
                                                typeInfo->typeId());
        d->selectInTypeModel->appendList(&infos);
    }
}

void InfoManager::doAddType(const QString &typeName, int type,
                          const QString &parentId, const QString &icon)
{
    Q_D(InfoManager);
    TypeInfo* info = new TypeInfo(this);
    emit d->worker->addType(info, typeName, type, parentId, icon);
}

void InfoManager::doAddTypeFinished(TypeInfo *info, int type,
                                    const QString &parentId)
{
    TypeModel * typeModel = NULL;

    if (KA::OUT == type) {
        if (parentId == KA::TOP_TYPE_ID) {
            typeModel = TopOutModel;
        } else {
            typeModel = ChildOutModel;
        }
    } else {
        if (parentId == KA::TOP_TYPE_ID) {
            typeModel = TopInModel;
        } else {
            typeModel = ChildInModel;
        }
    }

    if (NULL != typeModel) {
        typeModel->append(info);

        emit addTypeFinished(type, parentId, typeModel->count());
    }
}

void InfoManager::doDeleteTypeFinished(int index, int type,
                                       const QString &typeId,
                                       const QString &parentId)
{
    Q_UNUSED(typeId);
    TypeModel * typeModel = NULL;
    if (KA::OUT == type) {
        if (parentId == KA::TOP_TYPE_ID) {
            typeModel = TopOutModel;
        } else {
            typeModel = ChildOutModel;
        }
    } else {
        if (parentId == KA::TOP_TYPE_ID) {
            typeModel = TopInModel;
        } else {
            typeModel = ChildInModel;
        }
    }
    if (NULL != typeModel) {
        typeModel->remove(index);
        emit deleteTypeFinished();
    }
}

void InfoManager::initRecordData()
{
    if (!INIT_RECORD_DATA_FINISHED) {
        const QDate currentDate = QDate::currentDate();
        QObjectList datas = KA_DB->getRecordItems(
                    currentDate.year(), currentDate.month(), this);
        CurrentRecordModel->clear();
        CurrentRecordModel->set(&datas);

        INIT_RECORD_DATA_FINISHED = true;
    }
    emit initRecordFinished();
}

void InfoManager::doAddRecord(int type, const QString &parentId,
                              const QString &typeId, const QString &dateTime,
                              double amount, const QString &note,
                              const QString &icon)
{
    Q_D(InfoManager);
    RecordItem* item = new RecordItem(this);
    emit d->worker->addRecord(item, type, parentId, typeId,
                              dateTime, amount, note, icon);
}

void InfoManager::doAddRecordFinished(RecordItem *item)
{
    if (NULL != CurrentRecordModel) {
        CurrentRecordModel->push_front(item);
        emit addRecordFinished();
    }
}

void InfoManager::doDeleteRecordFinished(quint64 millonSecs)
{
    if (NULL != CurrentRecordModel) {
        CurrentRecordModel->deleteRecord(millonSecs);
        emit deleteRecordFinished();
    }
}

void InfoManager::doUpdateRecord(int recordIndex, int type,
                                 const QString &parentId, const QString &typeId,
                                 const QString &dateTime, double amount,
                                 const QString &note, const QString &icon)
{
    qDebug() << __FUNCTION__;
    Q_D(InfoManager);
    RecordItem *item
            = qobject_cast<RecordItem*>(CurrentRecordModel->get(recordIndex));
    emit d->worker->updateRecord(item, recordIndex, type, parentId, typeId,
                                 dateTime, amount, note, icon);
}

void InfoManager::doUpdateRecordFinished(RecordItem *item, int recordIndex)
{
    qDebug() << __FUNCTION__;
    if (NULL != CurrentRecordModel) {
        CurrentRecordModel->replace(recordIndex, item);
        emit updateRecordFinished();
    }
}

void InfoManagerPrivate::init()
{
    Q_Q(InfoManager);
    if (TopInModel == NULL) {
        TopInModel = new TypeModel(q);
    }
    if (ChildInModel == NULL) {
        ChildInModel = new TypeModel(q);
    }
    if (TopOutModel == NULL) {
        TopOutModel = new TypeModel(q);
    }
    if (ChildOutModel == NULL) {
        ChildOutModel = new TypeModel(q);
    }
    if (CurrentRecordModel == NULL) {
        CurrentRecordModel = new RecordModel(q);
    }
    if (worker == NULL) {
        worker = new Worker;
    }
    if (selectOutTypeModel == NULL) {
        selectOutTypeModel = new TypeModel(q);
    }
    if (selectInTypeModel == NULL) {
        selectInTypeModel = new TypeModel(q);
    }
}

void InfoManagerPrivate::uninit()
{
    if (TopInModel) {
        TopInModel->deleteLater();
        TopInModel = NULL;
    }
    if (ChildInModel) {
        ChildInModel->deleteLater();
        ChildInModel = NULL;
    }
    if (TopOutModel) {
        TopOutModel->deleteLater();
        TopOutModel = NULL;
    }
    if (ChildOutModel) {
        ChildOutModel->deleteLater();
        ChildOutModel = NULL;
    }
    if (CurrentRecordModel) {
        CurrentRecordModel->deleteLater();
        CurrentRecordModel = NULL;
    }
    if (selectOutTypeModel) {
        selectOutTypeModel->deleteLater();
        selectOutTypeModel = NULL;
    }
    if (selectInTypeModel) {
        selectInTypeModel->deleteLater();
        selectInTypeModel = NULL;
    }
}

void Worker::doAddType(TypeInfo* info, const QString &typeName, int type,
                       const QString &parentId, const QString &icon)
{
    info->setTypeName(typeName);
    info->setType(type);
    info->setTypeId(KA_UUID->createUuidV5());
    info->setIcon(icon);
    info->setParentId(parentId);
    info->setMillonSecs(QDateTime::currentMSecsSinceEpoch());
    info->setIndex(QString::number(GetCount(type, parentId)));

    bool success = KA_DB->addTypeInfo(info);

    qDebug() << __FUNCTION__ << typeName << type << parentId << success;

    if (success) {
        emit addTypeFinished(info, type, parentId);
    }
}

void Worker::doDeleteType(int index, int type, const QString &typeId,
                          const QString &parentId)
{
    bool success = KA_DB->deleteTypeInfoByTypeId(typeId);
    if (parentId == KA::TOP_TYPE_ID) {
        success = KA_DB->deleteTypeInfoByParentId(typeId);
    }
    if (success) {
        emit deleteTypeFinished(index, type, typeId, parentId);
    }
}

void Worker::doAddRecord(RecordItem* item, int type, const QString &parentId,
                         const QString &typeId, const QString &dateTime,
                         double amount, const QString &note, const QString &icon)
{
    setRecordItemData(item, type, parentId, typeId, dateTime, amount, note, icon);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    item->setMillonSecs(currentDateTime.toMSecsSinceEpoch());

    bool success = KA_DB->addRecordData(item);

    qDebug() << __FUNCTION__ << success;

    if (success) {
        emit addRecordFinished(item, type, parentId);
    }
}

void Worker::doDeleteRecord(quint64 millonSecs)
{
    bool success = KA_DB->deleteRecord(millonSecs);
    if (success) {
        emit deleteRecordFinished(millonSecs);
    }
}

void Worker::doUpdateRecord(RecordItem *item, int recordIndex, int type,
                            const QString &parentId, const QString &typeId,
                            const QString &dateTime, double amount,
                            const QString &note, const QString &icon)
{
    setRecordItemData(item, type, parentId, typeId, dateTime, amount, note, icon);

    bool success = KA_DB->updateRecordData(item);

    qDebug() << __FUNCTION__ << success;

    if (success) {
        emit updateRecordFinished(item, recordIndex);
    }
}

void Worker::setRecordItemData(RecordItem *item, int type,
                               const QString &parentId, const QString &typeId,
                               const QString &dateTime, double amount,
                               const QString &note, const QString &icon)
{
    item->setType(type);
    item->setAmount(amount);
    item->setNote(note);
    item->setIcon(icon);
    item->setParentType(parentId);
    item->setChildType(typeId);

    QStringList dateInfos = dateTime.split(KA::DATE_SEPARATOR);
    qDebug() << __FUNCTION__ << dateTime << dateInfos.size();
    if (dateInfos.size() > 0) {
        QString value = dateInfos.at(0);
        item->setYear(value.toInt());
        value = dateInfos.at(1);
        item->setMonth(value.toInt());
        value = dateInfos.at(2);
        item->setDay(value.toInt());
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();

    currentDateTime.setDate(QDate(item->year(), item->month(), item->day()));
    item->setDateTime(currentDateTime.toString(KA::DATE_TIME_FORMAT));
}
