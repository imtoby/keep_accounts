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

#include "DBManager.h"

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
            worker, &InitTypeInfoWorker::doWork);
    connect(worker, &InitTypeInfoWorker::resultReady,
            this, &TypeManager::initTypeInfoFinished);
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

void TypeManager::initData()
{
    emit startInitTypeInfo();
}

void InitTypeInfoWorker::doWork()
{
    IncomeTypeTopList = KA_DB->getType(KA::IN);

    QString topTypeId;
    QList<TypeItem> childrenList;

    for (const TypeItem& typeItem : IncomeTypeTopList) {
        topTypeId = typeItem.typeId();
        childrenList = KA_DB->getType(KA::IN, topTypeId);
        IncomeTypeChildrenMap.insert(topTypeId, childrenList);
    }

    ExpensesTypeTopList = KA_DB->getType(KA::OUT);

    for (const TypeItem& typeItem : ExpensesTypeTopList) {
        topTypeId = typeItem.typeId();
        childrenList = KA_DB->getType(KA::OUT, topTypeId);
        IncomeTypeChildrenMap.insert(topTypeId, childrenList);
    }

    emit resultReady();
}
