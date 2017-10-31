/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/30
    File: TypeManager.h
 ***************************************************************************/
#include "TypeManager.h"
#include <QList>

#include "TypeItem.h"
#include "DBManager.h"

class TypeManagerPrivate
{
public:
    TypeManagerPrivate()
    {
    }

    QList<TypeItem> incomeTypeTopList;
    QMap<QString, QList<TypeItem>> incomeTypeChildrenMap;
    QList<TypeItem> expensesTypeTopList;
    QMap<QString, QList<TypeItem>> expensesTypeChildrenMap;
};

TypeManager::TypeManager(QObject *parent)
    : QObject(parent)
    , d(new TypeManagerPrivate())
{
}

TypeManager::~TypeManager()
{
    if (d) {
        delete d;
        d = 0;
    }
}

void TypeManager::initData()
{
    initIncomeTypeList();
    initExpensesTypeList();
}

void TypeManager::initIncomeTypeList()
{
    d->incomeTypeTopList = KA_DB->getType(KA::IN);
    // todo init children
}

void TypeManager::initExpensesTypeList()
{
    d->expensesTypeTopList = KA_DB->getType(KA::OUT);
}
