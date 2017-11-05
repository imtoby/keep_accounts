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
#include <QStringList>

#include "TypeItem.h"

class TypeManagerPrivate;

class InitTypeInfoWorker : public QObject
{
    Q_OBJECT
public slots:
    void doInitWork();
    void doAddWork(const TypeItem &typeItem);

signals:
    void initReady();
    void addFinished();
};

class TypeManager : public QObject
{
    Q_OBJECT
public:
    TypeManager(QObject *parent = nullptr);
    ~TypeManager();

    Q_INVOKABLE QString createId() const;
    Q_INVOKABLE quint64 currentMillonSecs() const;
    Q_INVOKABLE int getCount(int type, const QString& parentId = QString());

    Q_INVOKABLE QStringList getTypeInfo(int type, const QString& parentId = QString());
    Q_INVOKABLE void addType(const QString& typeName, int type,
                             const QString& parentId = QString(),
                             const QString& icon = QString());

public slots:
    void initData();
    void doAddTypeFinished();

signals:
    void startInitTypeInfo();
    void startAddType(const TypeItem &typeItem);
    void initTypeInfoFinished();
    void addTopTypeFinished(const QString& typeName, const QString& typeId);
    void addChildTypeFinished(const QString& typeName, const QString& typeId);

private:
    QList<TypeItem> getTypeItems(int type, const QString& parentId = QString());

private:
    TypeManagerPrivate *d;
};

#endif // TYPEMANAGER_H_65C4F18A_DC47_515A_A0ED_3A4D1547F1E8
