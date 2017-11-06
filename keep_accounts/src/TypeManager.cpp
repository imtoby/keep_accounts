/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/30
    File: TypeManager.h
 ***************************************************************************/
#include "TypeManager.h"
#include <QThread>
#include <QMap>
#include <QDateTime>
#include <QDebug>

#include "DBManager.h"
#include "ZUuid.h"

namespace {

QList<TypeItem> IncomeTypeTopList;
QMap<QString, QList<TypeItem>> IncomeTypeChildrenMap;
QList<TypeItem> ExpensesTypeTopList;
QMap<QString, QList<TypeItem>> ExpensesTypeChildrenMap;

void InitDataWork() {
    IncomeTypeTopList.clear();
    IncomeTypeChildrenMap.clear();
    IncomeTypeTopList = KA_DB->getType(KA::IN, KA::TOP_TYPE_ID);

    QString topTypeId;
    QList<TypeItem> childrenList;

    for (const TypeItem& typeItem : IncomeTypeTopList) {
        topTypeId = typeItem.typeId;
        childrenList = KA_DB->getType(KA::IN, topTypeId);
        IncomeTypeChildrenMap.insert(topTypeId, childrenList);
    }

    ExpensesTypeTopList.clear();
    ExpensesTypeChildrenMap.clear();
    ExpensesTypeTopList = KA_DB->getType(KA::OUT, KA::TOP_TYPE_ID);

    for (const TypeItem& typeItem : ExpensesTypeTopList) {
        topTypeId = typeItem.typeId;
        childrenList = KA_DB->getType(KA::OUT, topTypeId);
        ExpensesTypeChildrenMap.insert(topTypeId, childrenList);
    }
}

}

class TypeManagerPrivate
{
public:
    TypeManagerPrivate()
    {
    }

    QThread workerThread;
    TypeItem typeItem;
};

TypeManager::TypeManager(QObject *parent)
    : QObject(parent)
    , d(new TypeManagerPrivate())
{
    TypeManagerWorker *worker = new TypeManagerWorker;
    worker->moveToThread(&d->workerThread);
    connect(&d->workerThread, &QThread::finished,
            worker, &QObject::deleteLater);
    connect(this, &TypeManager::startInitTypeInfo,
            worker, &TypeManagerWorker::doInitWork);
    connect(this, &TypeManager::startAddType,
            worker, &TypeManagerWorker::doAddType);
    connect(this, &TypeManager::startSetTypeName,
            worker, &TypeManagerWorker::doSetTypeName);


    connect(worker, &TypeManagerWorker::initReady,
            this, &TypeManager::initTypeInfoFinished);
    connect(worker, &TypeManagerWorker::addTypeFinished,
            this, &TypeManager::doAddTypeFinished);
    connect(worker, &TypeManagerWorker::setTypeNameFinished,
            this, &TypeManager::setTypeNameFinished);
    d->workerThread.start();
}

TypeManager::~TypeManager()
{
    d->workerThread.quit();
    d->workerThread.wait();
    if (d) {
        delete d;
        d = 0;
    }
}

QString TypeManager::createId() const
{
    return KA_UUID->createUuidV5();
}

quint64 TypeManager::currentMillonSecs() const
{
    return QDateTime::currentMSecsSinceEpoch();
}

int TypeManager::getCount(int type, const QString &parentId)
{
    if (KA::IN == type) {
        return (parentId == KA::TOP_TYPE_ID
                ? IncomeTypeTopList.size()
                : IncomeTypeChildrenMap.value(parentId).size());
    } else {
        return (parentId == KA::TOP_TYPE_ID
                ? ExpensesTypeTopList.size()
                : ExpensesTypeChildrenMap.value(parentId).size());
    }
}

QStringList TypeManager::getTypeInfo(int type, const QString &parentId)
{
    qDebug() << __FUNCTION__ << type << parentId;
    QStringList list;
    QList<TypeItem> listItems = getTypeItems(type, parentId);
    qDebug() << __FUNCTION__ << listItems.size();
    for (const TypeItem& typeItem : listItems) {
        QString item;
        item.append(typeItem.typeId);
        item.append(typeItem.typeName);
        list.push_back(item);
    }
    qDebug() << __FUNCTION__ << list;
    return list;
}

void TypeManager::addType(const QString &typeName, int type,
                          const QString &parentId, const QString &icon)
{
    qDebug() << __FUNCTION__ << typeName << type << parentId;
    d->typeItem.typeName = typeName;
    d->typeItem.type = KA::InorOut(type);
    d->typeItem.typeId = KA_UUID->createUuidV5();
    d->typeItem.icon = icon;
    d->typeItem.parentId = parentId;
    d->typeItem.millonSecs = QDateTime::currentMSecsSinceEpoch();
    d->typeItem.index = QString::number(getCount(type, parentId));

    emit startAddType(d->typeItem);
}

void TypeManager::setTypeName(const QString &typeId, const QString &typeName)
{
    emit startSetTypeName(typeId, typeName);
}

void TypeManager::initData()
{
    emit startInitTypeInfo();
}

void TypeManager::doAddTypeFinished()
{
    if (KA::IN == d->typeItem.type) {
        if (d->typeItem.parentId == KA::TOP_TYPE_ID){
            IncomeTypeTopList.push_back(d->typeItem);
            qDebug() << __FUNCTION__ << "KA::IN addTopTypeFinished";
            emit addTopTypeFinished(d->typeItem.typeName, d->typeItem.typeId);
        } else {
            IncomeTypeChildrenMap[d->typeItem.parentId].push_back(d->typeItem);
            qDebug() << __FUNCTION__ << "KA::IN addChildTypeFinished";
            emit addChildTypeFinished(d->typeItem.typeName, d->typeItem.typeId);
        }
    } else {
        if (d->typeItem.parentId == KA::TOP_TYPE_ID){
            ExpensesTypeTopList.push_back(d->typeItem);
            qDebug() << __FUNCTION__ << "KA::OUT addTopTypeFinished";
            emit addTopTypeFinished(d->typeItem.typeName, d->typeItem.typeId);
        } else {
            ExpensesTypeChildrenMap[d->typeItem.parentId].push_back(d->typeItem);
            qDebug() << __FUNCTION__ << "KA::OUT addChildTypeFinished";
            emit addChildTypeFinished(d->typeItem.typeName, d->typeItem.typeId);
        }
    }
}

QList<TypeItem> TypeManager::getTypeItems(int type, const QString &parentId)
{
    qDebug() << __FUNCTION__ << type << parentId;
    if (KA::IN == type) {
        if (parentId == KA::TOP_TYPE_ID) {
            qDebug() << __FUNCTION__ << "IncomeTypeTopList size:"
                     << IncomeTypeTopList.size();
            return IncomeTypeTopList;
        } else {
            return IncomeTypeChildrenMap.value(parentId);
        }
    } else {
        if (parentId == KA::TOP_TYPE_ID) {
            qDebug() << __FUNCTION__ << "ExpensesTypeTopList size:"
                     << ExpensesTypeTopList.size();
            return ExpensesTypeTopList;
        } else {
            return ExpensesTypeChildrenMap.value(parentId);
        }
    }
}

void TypeManagerWorker::doInitWork()
{
    qDebug() << __FUNCTION__;
    InitDataWork();
    qDebug() << __FUNCTION__
             << IncomeTypeTopList.size()
             << ExpensesTypeTopList.size();

    emit initReady();
}

void TypeManagerWorker::doAddType(const TypeItem &typeItem)
{
    bool success = KA_DB->addTypeData(typeItem);
    if (success) {
        emit addTypeFinished();
    } else {
        qDebug() << __FUNCTION__ << "doAddType failed";
    }
}

void TypeManagerWorker::doSetTypeName(const QString &typeId,
                                       const QString &typeName)
{
    bool success = KA_DB->updateTypeData(typeId, KA::TYPE_NAME, typeName);
    if (success) {
        emit setTypeNameFinished(typeName);
        InitDataWork();
    } else {
        qDebug() << __FUNCTION__ << "doSetTypeName failed";
    }
}
