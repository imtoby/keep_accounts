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

void DBManager::addTypeData(const TypeItem &typeItem)
{
    qDebug() << __FUNCTION__;

    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);
        bool check = query.prepare(KA::TABLE_TYPE_INSERT);

        if(check){
            query.bindValue(0, typeItem.typeId());
            query.bindValue(1, typeItem.type());
            query.bindValue(2, typeItem.typeName());
            query.bindValue(3, typeItem.index());
            query.bindValue(4, typeItem.millonSecs());
            query.bindValue(5, typeItem.icon());
            query.bindValue(6, typeItem.parentId());
            query.exec();
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << " Error: " << query.lastError();
        }

        query.clear();
        db.close();
    }
}

void DBManager::updateTypeData(const QString &typeId, const QString &key,
                               const QString &value)
{
    qDebug() << __FUNCTION__;

    if (KA::TYPE_ITEM_CONTENT.contains(key)) {

        QSqlDatabase db = database();
        if(db.open()){
            QSqlQuery query(db);

            const QString table_type_update("update "
                                            + KA::DATABASE_TABLE_NAME_TYPE
                                            + " set " + key + "=? where "
                                            + KA::ID + "=?");

            bool check = query.prepare(table_type_update);

            if(check){
                query.bindValue(0, value);
                query.bindValue(1, typeId);
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

QList<TypeItem> DBManager::getType(KA::InorOut inorOut,
                                   const QString &parentId)
{
    if (typeCount(parentId) > 0) {
        QSqlDatabase db = database();
        if(db.open()){
            QSqlQuery query(db);
            query.exec(QString("select "
                               + KA::ID           + ","
                               + KA::TYPE         + ","
                               + KA::TYPE_NAME    + ","
                               + KA::INDEX        + ","
                               + KA::MILLON_SECS  + ","
                               + KA::ICON         + ","
                               + KA::PARENT_ID    +
                               " from "     + KA::DATABASE_TABLE_NAME_TYPE +
                               " where "    + KA::TYPE +
                               "='%1'"
                               " and "      + KA::PARENT_ID +
                               "='%2' order by " + KA::MILLON_SECS +
                               " ASC")
                       .arg(QString::number(inorOut))
                       .arg(parentId));

            QList<TypeItem> list;
            while (query.next()) {
                TypeItem item;
                item.setTypeId(     query.value(0).toString());
                item.setType(       query.value(1).toInt());
                item.setTypeName(   query.value(2).toString());
                item.setIndex(      query.value(3).toString());
                item.setMillonSecs( query.value(4).toULongLong());
                item.setIcon(       query.value(5).toString());
                item.setParentId(   query.value(6).toString());
                list.push_back(item);
            }

            if(query.lastError().isValid()){
                qDebug() << __FUNCTION__ << query.lastError();
            }
            query.clear();
            db.close();

            return list;
        }
    }
    return QList<TypeItem>();
}

int DBManager::typeCount(const QString &parentId) const
{
    qDebug() << __FUNCTION__;

    int count = 0;

    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);
        query.exec(QString("select count(*) from "
                           + KA::DATABASE_TABLE_NAME_TYPE +
                           " where " + KA::PARENT_ID + "='%1'").arg(parentId));

        while (query.next()) {
            count = query.value(0).toInt();
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << query.lastError();
        }
        query.clear();
        db.close();

        return count;
    }

    return count;
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
