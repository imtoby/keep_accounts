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

class TypeManagerWorker : public QObject
{
    Q_OBJECT
public slots:
    void doInitWork();
    void doAddType(const TypeItem &typeItem);
    void doSetTypeName(const QString& typeId, const QString& typeName);

signals:
    void initReady();
    void addTypeFinished();
    void setTypeNameFinished(const QString& newTypeName);
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
    Q_INVOKABLE void setTypeName(const QString& typeId, const QString& typeName);

public slots:
    void initData();

private slots:
    void doAddTypeFinished();

signals:
    void startInitTypeInfo();
    void startAddType(const TypeItem &typeItem);
    void startSetTypeName(const QString& typeId, const QString& typeName);

    void initTypeInfoFinished();
    void addTopTypeFinished(const QString& typeName, const QString& typeId);
    void addChildTypeFinished(const QString& typeName, const QString& typeId);
    void setTypeNameFinished(const QString& newTypeName);

private:
    QList<TypeItem> getTypeItems(int type, const QString& parentId = QString());

private:
    TypeManagerPrivate *d;
};

#endif // TYPEMANAGER_H_65C4F18A_DC47_515A_A0ED_3A4D1547F1E8
