/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/30
    File: TypeManager.h
 ***************************************************************************/
#ifndef TYPEMANAGER_H_65C4F18A_DC47_515A_A0ED_3A4D1547F1E8
#define TYPEMANAGER_H_65C4F18A_DC47_515A_A0ED_3A4D1547F1E8

#include <QObject>

class TypeManagerPrivate;
class TypeModel;

class TypeManager
{
    Q_OBJECT
public:
    TypeManager(QObject *parent = nullptr);
    ~TypeManager();

    void initData();

private:
    void initIncomeTypeList();
    void initExpensesTypeList();

private:
    TypeManagerPrivate *d;
};

#endif // TYPEMANAGER_H_65C4F18A_DC47_515A_A0ED_3A4D1547F1E8
