#include "DBManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "ConfigInfo.h"
#include "RecordItem.h"

namespace {

QString DatabaseDirPath     = "";
QString DatabaseFileName    = "";

}

class DBManagerPrivate
{
public:
    DBManagerPrivate() {}
};

DBManager::DBManager(QObject *parent)
    : QObject(parent)
    , d(new DBManagerPrivate)
{
    DatabaseDirPath = QCoreApplication::applicationDirPath() +
            QDir::separator() + KA::DATABASE_DIR_NAME;
    DatabaseFileName = DatabaseDirPath + QDir::separator() + KA::DATABASE_NAME;

    createDatabase();
}

DBManager::~DBManager()
{
    if(d){
        delete d;
        d = 0;
    }
}

void DBManager::addRecordData(const RecordItem &recordItem)
{
    qDebug() << __FUNCTION__;

    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);
        bool check = query.prepare(KA::TABLE_RECORDS_INSERT);

        if(check){
            query.bindValue(0, recordItem.millonSecs());
            query.bindValue(1, recordItem.dateTime());
            query.bindValue(2, recordItem.year());
            query.bindValue(3, recordItem.month());
            query.bindValue(4, recordItem.day());
            query.bindValue(5, recordItem.type());
            query.bindValue(6, recordItem.parentType());
            query.bindValue(7, recordItem.childType());
            query.bindValue(8, recordItem.amount());
            query.bindValue(9, recordItem.note());
            query.bindValue(10, recordItem.icon());
            query.exec();
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << " Error: " << query.lastError();
        }
        query.clear();
        db.close();
    }
}

void DBManager::updateRecordData(const QString &millonSecs, const QString &key,
                                 const QString &value)
{
    qDebug() << __FUNCTION__;
    if (KA::RECORD_ITEM_CONTENT.contains(key)) {

        QSqlDatabase db = database();
        if(db.open()){
            QSqlQuery query(db);

            const QString table_records_update("update "
                                               + KA::DATABASE_TABLE_NAME_RECORDS
                                               + " set " + key + "=? where "
                                               + KA::MILLON_SECS + "=?");

            bool check = query.prepare(table_records_update);

            if(check){
                query.bindValue(0, value);
                query.bindValue(1, millonSecs);
                query.exec();
            }

            if(query.lastError().isValid()){
                qDebug() << __FUNCTION__ << query.lastError();
            }
            query.clear();
            db.close();
        }

    } else {
        qDebug() << __FUNCTION__ << " Error: key is not exsit.";
    }
}

QSqlDatabase DBManager::database() const
{
    QSqlDatabase db;
    if (QSqlDatabase::contains(KA::DATABASE_BASE_NAME)) {
        db = QSqlDatabase::database(KA::DATABASE_BASE_NAME);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", KA::DATABASE_BASE_NAME);
        db.setDatabaseName(DatabaseFileName);
    }
    return db;
}

void DBManager::createDatabase() const
{
    qDebug() << QSqlDatabase::drivers();
    if(!QFile::exists(DatabaseFileName)){
        qDebug() << "Database File: \"" << DatabaseFileName
                 << "\" is not exist, Creating...";

        QDir dbDir(DatabaseDirPath);
        if(!dbDir.exists()){
            qDebug() << "Database Dir Path: \"" << DatabaseDirPath
                     << "\" is not exist, Creating...";
            dbDir.mkpath(DatabaseDirPath);
        }

        QSqlDatabase qdb = QSqlDatabase::addDatabase("QSQLITE",
                                                     KA::DATABASE_BASE_NAME);
        qdb.setDatabaseName(DatabaseFileName);
        if (qdb.open()) {
            qdb.exec(KA::TABLE_RECORDS);
            qdb.exec(KA::TABLE_TYPE);
        }

        if(qdb.lastError().isValid()){
            qDebug() << "lastError: " << qdb.lastError().text();
        }

        qdb.close();
    }
}
