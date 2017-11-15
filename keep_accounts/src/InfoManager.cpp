#include "InfoManager.h"
#include "DBManager.h"
#include "TypeInfo.h"
#include "ZUuid.h"
#include "base/kglobal.h"

#include <QDebug>
#include <QDateTime>

class InfoManagerPrivate
{
public:
    InfoManagerPrivate(InfoManager *parent)
        : q_ptr(parent)
        , topInModel(NULL)
        , childInModel(NULL)
        , topOutModel(NULL)
        , childOutModel(NULL)
    {}

    void init();
    void uninit();

private:
    InfoManager * const q_ptr;
    Q_DECLARE_PUBLIC(InfoManager)

    TypeModel* topInModel;
    TypeModel* childInModel;
    TypeModel* topOutModel;
    TypeModel* childOutModel;
};

InfoManager::InfoManager(QObject *parent)
    : QObject(parent)
    , d_ptr(new InfoManagerPrivate(this))
{
    Q_D(InfoManager);
    d->init();

    connect(this, &InfoManager::addType, this, &InfoManager::doAddType);
    connect(this, &InfoManager::deleteType, this, &InfoManager::doDeleteType);
}

InfoManager::~InfoManager()
{
    Q_D(InfoManager);
    d->uninit();
}

void InfoManager::initData()
{
    initTypeData();
}

TypeModel *InfoManager::typeModel(int type, const QString& parentId)
{
    Q_D(InfoManager);

    if (KA::OUT == type) {
        if (parentId == KA::TOP_TYPE_ID) {
            return d->topOutModel;
        } else {
            QObjectList infos = KA_DB->getTypeInfos(KA::OUT, this, parentId);
            d->childOutModel->clear();
            d->childOutModel->set(&infos);
            return d->childOutModel;
        }
    } else {
        if (parentId == KA::TOP_TYPE_ID) {
            return d->topInModel;
        } else {
            QObjectList infos = KA_DB->getTypeInfos(KA::IN, this, parentId);
            d->childInModel->clear();
            d->childInModel->set(&infos);
            return d->childInModel;
        }
    }

    return d->topOutModel;
}

void InfoManager::setTypeName(int type, const QString &typeId,
                              const QString &parentId,
                              const QString &typeName)
{
    Q_D(InfoManager);
    bool success = KA_DB->updateTypeInfo(typeId, KA::TYPE_NAME, typeName);
    if (success) {
        TypeInfo* info = NULL;
        if (KA::OUT == type) {
            if (parentId == KA::TOP_TYPE_ID) {
                info = d->topOutModel->getTypeInfoByTypeId(typeId);
            } else {
                info = d->childOutModel->getTypeInfoByTypeId(typeId);
            }
        } else {
            if (parentId == KA::TOP_TYPE_ID) {
                info = d->topInModel->getTypeInfoByTypeId(typeId);
            } else {
                info = d->childInModel->getTypeInfoByTypeId(typeId);
            }
        }
        if (NULL != info) {
            info->setTypeName(typeName);
        }
    } else {
        qDebug() << __FUNCTION__ << "doSetTypeName failed";
    }
}

void InfoManager::doAddType(const QString &typeName, int type,
                          const QString &parentId, const QString &icon)
{
    Q_D(InfoManager);
    TypeInfo* info = new TypeInfo(this);

    info->setTypeName(typeName);
    info->setType(type);
    info->setTypeId(KA_UUID->createUuidV5());
    info->setIcon(icon);
    info->setParentId(parentId);
    info->setMillonSecs(QDateTime::currentMSecsSinceEpoch());
    info->setIndex(QString::number(getCount(type, parentId)));

    bool success = KA_DB->addTypeInfo(info);

    qDebug() << __FUNCTION__ << typeName << type << parentId << success;

    if (success) {
        TypeModel * typeModel = NULL;

        if (KA::OUT == type) {
            if (parentId == KA::TOP_TYPE_ID) {
                typeModel = d->topOutModel;
            } else {
                typeModel = d->childOutModel;
            }
        } else {
            if (parentId == KA::TOP_TYPE_ID) {
                typeModel = d->topInModel;
            } else {
                typeModel = d->childInModel;
            }
        }

        if (NULL != typeModel) {
            typeModel->append(info);
            emit addTypeFinished(type, parentId, typeModel->count());
        }
    }

}

void InfoManager::doDeleteType(int index, int type, const QString &typeId,
                               const QString &parentId)
{
    Q_D(InfoManager);
    bool success = KA_DB->deleteTypeInfo(typeId);
    if (success) {
        TypeModel * typeModel = NULL;
        if (KA::OUT == type) {
            if (parentId == KA::TOP_TYPE_ID) {
                typeModel = d->topOutModel;
            } else {
                typeModel = d->childOutModel;
            }
        } else {
            if (parentId == KA::TOP_TYPE_ID) {
                typeModel = d->topInModel;
            } else {
                typeModel = d->childInModel;
            }
        }
        if (NULL != typeModel) {
            typeModel->remove(index);
            emit deleteTypeFinished();
        }
    }
}

void InfoManager::initTypeData()
{
    Q_D(InfoManager);
    QObjectList infos = KA_DB->getTypeInfos(KA::IN, this, KA::TOP_TYPE_ID);
    d->topInModel->clear();
    d->topInModel->set(&infos);

    infos = KA_DB->getTypeInfos(KA::OUT, this, KA::TOP_TYPE_ID);
    d->topOutModel->clear();
    d->topOutModel->set(&infos);

    emit initTypeFinished();
}

int InfoManager::getCount(int type, const QString &parentId) const
{
    C_D(InfoManager);
    if (KA::OUT == type) {
        if (parentId == KA::TOP_TYPE_ID) {
            return d->topOutModel->count();
        } else {
            return d->childOutModel->count();
        }
    } else {
        if (parentId == KA::TOP_TYPE_ID) {
            return d->topInModel->count();
        } else {
            return d->childInModel->count();
        }
    }
}

void InfoManagerPrivate::init()
{
    Q_Q(InfoManager);
    if (topInModel == NULL) {
        topInModel = new TypeModel(q);
    }
    if (childInModel == NULL) {
        childInModel = new TypeModel(q);
    }
    if (topOutModel == NULL) {
        topOutModel = new TypeModel(q);
    }
    if (childOutModel == NULL) {
        childOutModel = new TypeModel(q);
    }
}

void InfoManagerPrivate::uninit()
{
    if (topInModel) {
        topInModel->deleteLater();
        topInModel = NULL;
    }
    if (childInModel) {
        childInModel->deleteLater();
        childInModel = NULL;
    }
    if (topOutModel) {
        topOutModel->deleteLater();
        topOutModel = NULL;
    }
    if (childOutModel) {
        childOutModel->deleteLater();
        childOutModel = NULL;
    }
}
