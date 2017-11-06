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
#include "TypeItem.h"

class QSqlDatabase;
class RecordItem;

class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

signals:

public slots:
    void addRecordData(const RecordItem &recordItem);
    void updateRecordData(const QString &millonSecs, const QString &key,
                          const QString &value);

    bool addTypeData(const TypeItem &typeItem);
    bool updateTypeData(const QString &typeId, const QString &key,
                        const QString &value);

    QList<TypeItem> getType(KA::InorOut inorOut,
                            const QString& parentId = QString());

    int typeCount(const QString& parentId = QString()) const;

private:
    QSqlDatabase database() const;
    void createDatabase() const;
};

#define KA_DB CSingleton<DBManager>::instance()

#endif // DBMANAGER_H_5DF607D2_DA87_4954_A19E_8F541D2643E0
