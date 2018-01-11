/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/25
    File: DBManager.h
 ***************************************************************************/
#ifndef DBMANAGER_H_5DF607D2_DA87_4954_A19E_8F541D2643E0
#define DBMANAGER_H_5DF607D2_DA87_4954_A19E_8F541D2643E0

#include <QObject>
#include <QList>

#include "CSingleton.h"
#include "ConfigInfo.h"

class QSqlDatabase;
class RecordItem;
class TypeInfo;

class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

signals:

public slots:
    bool updateRecordData(quint64 millonSecs, const QString &key,
                          const QString &value);
    bool updateRecordData(const RecordItem* const recordItem);
    QObjectList getRecordItems(int year, int month, QObject *parent);
    QObjectList getRecordItems(int year, int month, int type, QObject *parent);
    bool addRecordData(const RecordItem* const recordItem);
    bool deleteRecord(quint64 millonSecs);
    QObjectList searchRecordItems(const QString& key) const;

    bool updateTypeInfo(const QString &typeId, const QString &key,
                        const QString &value);
    QObjectList getTypeInfos(KA::InorOut inorOut, QObject *parent,
                             const QString &parentId = QString());
    bool addTypeInfo(const TypeInfo* const typeItem);
    bool deleteTypeInfoByTypeId(const QString &typeId);
    bool deleteTypeInfoByParentId(const QString &parentId);
    int typeCount(const QString& parentId = QString()) const;

    QString getTypeName(const QString &typeId) const;

private:
    QSqlDatabase database() const;
    void createDatabase() const;
};

#define KA_DB CSingleton<DBManager>::instance()

#endif // DBMANAGER_H_5DF607D2_DA87_4954_A19E_8F541D2643E0
