#include "DBManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "RecordItem.h"
#include "TypeInfo.h"

namespace {

QString DatabaseDirPath     = "";
QString DatabaseFileName    = "";

}

DBManager::DBManager(QObject *parent)
    : QObject(parent)
{
    DatabaseDirPath = QCoreApplication::applicationDirPath() +
            QDir::separator() + KA::DATABASE_DIR_NAME;
    DatabaseFileName = DatabaseDirPath + QDir::separator() + KA::DATABASE_NAME;

    createDatabase();
}

DBManager::~DBManager()
{
}

bool DBManager::updateRecordData(quint64 millonSecs, const QString &key,
                                 const QString &value)
{
    qDebug() << __FUNCTION__ << millonSecs;
    bool success = false;
    if (KA::RECORD_ITEM_CONTENT.contains(key)) {
        QSqlDatabase db = database();
        if(db.open()){
            db.transaction();
            QSqlQuery query(db);

            const QString table_records_update("update "
                                               + KA::DATABASE_TABLE_NAME_RECORDS
                                               + " set " + key + " = :theKey where "
                                               + KA::MILLON_SECS + " = :theMSecs");

            bool check = query.prepare(table_records_update);

            if(check){
                query.bindValue(":theKey", value);
                query.bindValue(":theMSecs", QString::number(millonSecs));
                query.exec();
            }

            if(query.lastError().isValid()){
                qDebug() << __FUNCTION__ << query.lastError();
                db.rollback();
            } else {
                success = true;
                db.commit();
            }

            query.clear();
            db.close();
        } else {
            qDebug() << __FUNCTION__ << db.lastError();
        }
    } else {
        qDebug() << __FUNCTION__ << QStringLiteral(" Error: key is not exsit.");
    }
    return success;
}

bool DBManager::updateRecordData(const RecordItem * const recordItem)
{
    return updateRecordData(recordItem->millonSecs(), KA::DATE_TIME,
                               recordItem->dateTime())
            && updateRecordData(recordItem->millonSecs(), KA::YEAR,
                                          QString::number(recordItem->year()))
            && updateRecordData(recordItem->millonSecs(), KA::MONTH,
                                          QString::number(recordItem->month()))
            && updateRecordData(recordItem->millonSecs(), KA::DAY,
                                          QString::number(recordItem->day()))
            && updateRecordData(recordItem->millonSecs(), KA::TYPE,
                                          QString::number(recordItem->type()))
            && updateRecordData(recordItem->millonSecs(), KA::PARENT_TYPE,
                                          recordItem->parentType())
            && updateRecordData(recordItem->millonSecs(), KA::CHILD_TYPE,
                                          recordItem->childType())
            && updateRecordData(recordItem->millonSecs(), KA::AMOUNT,
                                          QString::number(recordItem->amount()))
            && updateRecordData(recordItem->millonSecs(), KA::NOTE,
                                          recordItem->note())
            && updateRecordData(recordItem->millonSecs(), KA::ICON,
                                          recordItem->icon());
}

QObjectList DBManager::getRecordItems(int year, int month, QObject *parent)
{
    QSqlDatabase db = database();
    if(db.open()) {
        QSqlQuery query(db);
        const QString sqlString
                = QString("select "
                          + KA::MILLON_SECS + ","
                          + KA::DATE_TIME   + ","
                          + KA::YEAR        + ","
                          + KA::MONTH       + ","
                          + KA::DAY         + ","
                          + KA::TYPE        + ","
                          + KA::PARENT_TYPE + ","
                          + KA::CHILD_TYPE  + ","
                          + KA::AMOUNT      + ","
                          + KA::NOTE        + ","
                          + KA::ICON        +
                          " from "          + KA::DATABASE_TABLE_NAME_RECORDS +
                          " where "         + KA::YEAR +
                          "='%1' and "      + KA::MONTH +
                          "='%2' order by " + KA::DATE_TIME +
                          " DESC")
                .arg(QString::number(year))
                .arg(QString::number(month));

        query.exec(sqlString);

        QObjectList list;
        while (query.next()) {
            RecordItem* item = new RecordItem(parent);
            item->setMillonSecs(    query.value(0).toULongLong());
            item->setDateTime(      query.value(1).toString());
            item->setYear(          query.value(2).toInt());
            item->setMonth(         query.value(3).toInt());
            item->setDay(           query.value(4).toInt());
            item->setType(          query.value(5).toInt());
            item->setParentType(    query.value(6).toString());
            item->setChildType(     query.value(7).toString());
            item->setAmount(        query.value(8).toDouble());
            item->setNote(          query.value(9).toString());
            item->setIcon(          query.value(10).toString());
            list.push_back(item);
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << query.lastError();
        }
        query.clear();
        db.close();

        qDebug() << __FUNCTION__
                 << QStringLiteral("list.size: ") << list.size();

        return list;
    }
    return QObjectList();
}

QObjectList DBManager::getRecordItems(int year, int month, int type, QObject *parent)
{
    QSqlDatabase db = database();
    if(db.open()) {
        QSqlQuery query(db);
        const QString sqlString
                = QString("select "
                          + KA::MILLON_SECS + ","
                          + KA::DATE_TIME   + ","
                          + KA::YEAR        + ","
                          + KA::MONTH       + ","
                          + KA::DAY         + ","
                          + KA::TYPE        + ","
                          + KA::PARENT_TYPE + ","
                          + KA::CHILD_TYPE  + ","
                          + KA::AMOUNT      + ","
                          + KA::NOTE        + ","
                          + KA::ICON        +
                          " from "          + KA::DATABASE_TABLE_NAME_RECORDS +
                          " where "         + KA::YEAR +
                          "='%1' and "      + KA::MONTH +
                          "='%2' and "      + KA::TYPE +
                          "='%3' order by " + KA::DATE_TIME +
                          " DESC")
                .arg(QString::number(year))
                .arg(QString::number(month))
                .arg(QString::number(type));

        query.exec(sqlString);

        QObjectList list;
        while (query.next()) {
            RecordItem* item = new RecordItem(parent);
            item->setMillonSecs(    query.value(0).toULongLong());
            item->setDateTime(      query.value(1).toString());
            item->setYear(          query.value(2).toInt());
            item->setMonth(         query.value(3).toInt());
            item->setDay(           query.value(4).toInt());
            item->setType(          query.value(5).toInt());
            item->setParentType(    query.value(6).toString());
            item->setChildType(     query.value(7).toString());
            item->setAmount(        query.value(8).toDouble());
            item->setNote(          query.value(9).toString());
            item->setIcon(          query.value(10).toString());
            list.push_back(item);
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << query.lastError();
        }
        query.clear();
        db.close();

        qDebug() << __FUNCTION__
                 << QStringLiteral("list.size: ") << list.size();

        return list;
    }
    return QObjectList();
}

bool DBManager::addRecordData(const RecordItem* const recordItem)
{
    qDebug() << __FUNCTION__;

    bool addSuccess = false;

    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);
        bool check = query.prepare(KA::TABLE_RECORDS_INSERT);

        if(check){
            query.bindValue(0,  recordItem->millonSecs());
            query.bindValue(1,  recordItem->dateTime());
            query.bindValue(2,  recordItem->year());
            query.bindValue(3,  recordItem->month());
            query.bindValue(4,  recordItem->day());
            query.bindValue(5,  recordItem->type());
            query.bindValue(6,  recordItem->parentType());
            query.bindValue(7,  recordItem->childType());
            query.bindValue(8,  recordItem->amount());
            query.bindValue(9,  recordItem->note());
            query.bindValue(10, recordItem->icon());
            query.exec();
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__
                     << QStringLiteral(" Error: ") << query.lastError();
        } else {
            addSuccess = true;
        }

        query.clear();
        db.close();
    }

    return addSuccess;
}

bool DBManager::deleteRecord(quint64 millonSecs)
{
    qDebug() << __FUNCTION__;

    bool success = false;

    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);

        const QString table_record_delete("delete from "
                                          + KA::DATABASE_TABLE_NAME_RECORDS
                                          + " where " + KA::MILLON_SECS + "=?");

        bool check = query.prepare(table_record_delete);

        if(check){
            query.bindValue(0, millonSecs);
            query.exec();
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << query.lastError();
        } else {
            success = true;
        }

        query.clear();
        db.close();
    }

    return success;
}

QObjectList DBManager::searchRecordItems(const QString &keys,
                                         QObject *parent) const
{
    if (!keys.isEmpty()) {
        QSqlDatabase db = database();
        if(db.open()){
            QSqlQuery query(db);

            QString sqlString = "SELECT * FROM "
                    + KA::DATABASE_TABLE_NAME_RECORDS
                    + " WHERE "
                    + KA::YEAR          + " LIKE '%" + keys + "%' OR "
                    + KA::MONTH         + " LIKE '%" + keys + "%' OR "
                    + KA::DAY           + " LIKE '%" + keys + "%' OR "
                    + KA::PARENT_TYPE   + " LIKE '%" + keys + "%' OR "
                    + KA::CHILD_TYPE    + " LIKE '%" + keys + "%' OR "
                    + KA::AMOUNT        + " LIKE '%" + keys + "%' OR "
                    + KA::NOTE          + " LIKE '%" + keys + "%'";

            query.exec(sqlString);
            QObjectList list;
            while (query.next()) {
                RecordItem* item = new RecordItem(parent);
                item->setMillonSecs(    query.value(0).toULongLong());
                item->setDateTime(      query.value(1).toString());
                item->setYear(          query.value(2).toInt());
                item->setMonth(         query.value(3).toInt());
                item->setDay(           query.value(4).toInt());
                item->setType(          query.value(5).toInt());
                item->setParentType(    query.value(6).toString());
                item->setChildType(     query.value(7).toString());
                item->setAmount(        query.value(8).toDouble());
                item->setNote(          query.value(9).toString());
                item->setIcon(          query.value(10).toString());
                list.push_back(item);
            }

            if(query.lastError().isValid()){
                qDebug() << __FUNCTION__ << query.lastError();
            }
            query.clear();
            db.close();

            qDebug() << __FUNCTION__
                     << QStringLiteral("list.size: ") << list.size();

            return list;
        }
    }
    return QObjectList();
}

bool DBManager::updateTypeInfo(const QString &typeId, const QString &key,
                               const QString &value)
{
    qDebug() << __FUNCTION__;

    bool success = false;

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
            } else {
                success = true;
            }

            query.clear();
            db.close();
        }

    } else {
        qDebug() << __FUNCTION__ << QStringLiteral(" Error: key is not exsit.");
    }

    return success;
}

QObjectList DBManager::getTypeInfos(KA::InorOut inorOut, QObject *parent,
                                    const QString &parentId)
{
    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);
        const QString sqlString
                = QString("select "
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
                .arg(parentId);

        query.exec(sqlString);

        QObjectList list;
        while (query.next()) {
            TypeInfo* item = new TypeInfo(parent);
            item->setTypeId(        query.value(0).toString());
            item->setType(          query.value(1).toInt());
            item->setTypeName(      query.value(2).toString());
            item->setIndex(         query.value(3).toString());
            item->setMillonSecs(    query.value(4).toULongLong());
            item->setIcon(          query.value(5).toString());
            item->setParentId(      query.value(6).toString());
            list.push_back(item);
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << query.lastError();
        }
        query.clear();
        db.close();

        qDebug() << __FUNCTION__
                 << QStringLiteral("list.size: ") << list.size();

        return list;
    }
    return QObjectList();
}

bool DBManager::addTypeInfo(const TypeInfo * const typeItem)
{
    qDebug() << __FUNCTION__;

    bool addSuccess = false;
    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);
        bool check = query.prepare(KA::TABLE_TYPE_INSERT);

        if(check){
            query.bindValue(0, typeItem->typeId());
            query.bindValue(1, typeItem->type());
            query.bindValue(2, typeItem->typeName());
            query.bindValue(3, typeItem->index());
            query.bindValue(4, typeItem->millonSecs());
            query.bindValue(5, typeItem->icon());
            query.bindValue(6, typeItem->parentId());
            query.exec();
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__
                     << QStringLiteral(" Error: ") << query.lastError();
        } else {
            addSuccess = true;
        }

        query.clear();
        db.close();
    }

    return addSuccess;
}

bool DBManager::deleteTypeInfoByTypeId(const QString &typeId)
{
    qDebug() << __FUNCTION__ << typeId;

    bool success = false;

    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);

        const QString table_type_delete("delete from "
                                        + KA::DATABASE_TABLE_NAME_TYPE
                                        + " where " + KA::ID + "=?");

        bool check = query.prepare(table_type_delete);

        if(check){
            query.bindValue(0, typeId);
            query.exec();
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << query.lastError();
        } else {
            success = true;
        }

        query.clear();
        db.close();
    }

    return success;
}

bool DBManager::deleteTypeInfoByParentId(const QString &parentId)
{
    qDebug() << __FUNCTION__ << parentId;

    bool success = false;

    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);

        const QString table_type_delete("delete from "
                                        + KA::DATABASE_TABLE_NAME_TYPE
                                        + " where " + KA::PARENT_ID + "=?");

        bool check = query.prepare(table_type_delete);

        if(check){
            query.bindValue(0, parentId);
            query.exec();
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << query.lastError();
        } else {
            success = true;
        }

        query.clear();
        db.close();
        return success;
    }

    return success;
}

int DBManager::typeCount(const QString &parentId) const
{
    qDebug() << __FUNCTION__;

    int count = 0;

    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);

        const QString sqlString
                = QString("select count(*) from "
                          + KA::DATABASE_TABLE_NAME_TYPE +
                          " where " + KA::PARENT_ID + "='%1'").arg(parentId);

        query.exec(sqlString);

        while (query.next()) {
            count = query.value(0).toInt();
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << query.lastError();
        }
        query.clear();
        db.close();
    }

    qDebug() << __FUNCTION__ << QStringLiteral("count") << count;

    return count;
}

QString DBManager::getTypeName(const QString &typeId) const
{
    qDebug() << __FUNCTION__
             << QStringLiteral("typeId: ") << typeId;

    QSqlDatabase db = database();
    if(db.open()){
        QSqlQuery query(db);
        const QString sqlString
                = QString("select " + KA::TYPE_NAME +
                          " from "  + KA::DATABASE_TABLE_NAME_TYPE +
                          " where " + KA::ID + "='%1' ").arg(typeId);

        query.exec(sqlString);

        QString typeName = QStringLiteral("");
        while (query.next()) {
            typeName = query.value(0).toString();
            break;
        }

        if(query.lastError().isValid()){
            qDebug() << __FUNCTION__ << query.lastError();
        }
        query.clear();
        db.close();

        qDebug() << __FUNCTION__
                 << QStringLiteral("typeName: ") << typeName;

        return typeName;
    }
    return QStringLiteral("");
}

QSqlDatabase DBManager::database() const
{
    QSqlDatabase db;
    if (QSqlDatabase::contains(KA::DATABASE_BASE_NAME)) {
        db = QSqlDatabase::database(KA::DATABASE_BASE_NAME);
    } else {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"),
                                       KA::DATABASE_BASE_NAME);
        db.setDatabaseName(DatabaseFileName);
    }
    return db;
}

void DBManager::createDatabase() const
{
    qDebug() << QSqlDatabase::drivers();
    if(!QFile::exists(DatabaseFileName)){
        qDebug() << QStringLiteral("Database File: \"")
                 << DatabaseFileName
                 << QStringLiteral("\" is not exist, Creating...");

        QDir dbDir(DatabaseDirPath);
        if(!dbDir.exists()){
            qDebug() << QStringLiteral("Database Dir Path: \"")
                     << DatabaseDirPath
                     << QStringLiteral("\" is not exist, Creating...");
            dbDir.mkpath(DatabaseDirPath);
        }

        QSqlDatabase qdb = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"),
                                                     KA::DATABASE_BASE_NAME);
        qdb.setDatabaseName(DatabaseFileName);

        if (qdb.open()) {
            qdb.exec(KA::TABLE_RECORDS);
            qdb.exec(KA::TABLE_TYPE);
        }

        if(qdb.lastError().isValid()){
            qDebug() << QStringLiteral("lastError: ") << qdb.lastError().text();
        }

        qdb.close();
    }
}
