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
    InitTypeInfoWorker *worker = new InitTypeInfoWorker;
    worker->moveToThread(&d->workerThread);
    connect(&d->workerThread, &QThread::finished,
            worker, &QObject::deleteLater);
    connect(this, &TypeManager::startInitTypeInfo,
            worker, &InitTypeInfoWorker::doInitWork);
    connect(this, &TypeManager::startAddType,
            worker, &InitTypeInfoWorker::doAddWork);
    connect(worker, &InitTypeInfoWorker::initReady,
            this, &TypeManager::initTypeInfoFinished);
    connect(worker, &InitTypeInfoWorker::addFinished,
            this, &TypeManager::doAddTypeFinished);
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
        return parentId.isEmpty()
                ? IncomeTypeTopList.size()
                : IncomeTypeChildrenMap.value(parentId).size();
    } else {
        return parentId.isEmpty()
                ? ExpensesTypeTopList.size()
                : ExpensesTypeChildrenMap.value(parentId).size();
    }
}

QStringList TypeManager::getTypeInfo(int type, const QString &parentId)
{
    qDebug() << __FUNCTION__ << type << parentId;
    QStringList list;
    QList<TypeItem> listItems = getTypeItems(type, parentId);
    for (const TypeItem& typeItem : listItems) {
        QString item;
        item.append(typeItem.typeId);
        item.append(typeItem.typeName);
        list.push_back(item);
    }
    return list;
}

void TypeManager::addType(const QString &typeName, int type,
                          const QString &parentId, const QString &icon)
{
    d->typeItem.typeName = typeName;
    d->typeItem.type = KA::InorOut(type);
    d->typeItem.typeId = KA_UUID->createUuidV5();
    d->typeItem.icon = icon;
    d->typeItem.parentId = parentId;
    d->typeItem.millonSecs = QDateTime::currentMSecsSinceEpoch();
    d->typeItem.index = QString::number(getCount(type, parentId));

    emit startAddType(d->typeItem);
}

void TypeManager::initData()
{
    emit startInitTypeInfo();
}

void TypeManager::doAddTypeFinished()
{
    emit addTypeFinished(d->typeItem.typeId);
}

QList<TypeItem> TypeManager::getTypeItems(int type, const QString &parentId)
{
    qDebug() << __FUNCTION__ << type << parentId;
    if (KA::IN == type) {
        return parentId.isEmpty()
                ? IncomeTypeTopList
                : IncomeTypeChildrenMap.value(parentId);
    } else {
        return parentId.isEmpty()
                ? ExpensesTypeTopList
                : ExpensesTypeChildrenMap.value(parentId);
    }
}

void InitTypeInfoWorker::doInitWork()
{
    qDebug() << __FUNCTION__;
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

    qDebug() << __FUNCTION__
             << IncomeTypeTopList.size()
             << ExpensesTypeTopList.size();

    emit initReady();
}

void InitTypeInfoWorker::doAddWork(const TypeItem &typeItem)
{
    KA_DB->addTypeData(typeItem);
}
