/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/25
    File: ConfigInfo.h
 ***************************************************************************/
#ifndef CONFIGINFO_H_986BE50A_F753_527F_9CD6_154434F55771
#define CONFIGINFO_H_986BE50A_F753_527F_9CD6_154434F55771

#include <QString>
#include <QStringList>

namespace KA {

enum InorOut {
    OUT = 0,
    IN
};

// for every record item
const QString MILLON_SECS   = "_MillonSecs";
const QString DATE_TIME     = "_DateTime";
const QString YEAR          = "_Year";
const QString MONTH         = "_Month";
const QString DAY           = "_Day";
const QString TYPE          = "_Type";
const QString PARENT_TYPE   = "_ParentType";
const QString CHILD_TYPE    = "_ChildType";
const QString AMOUNT        = "_Amount";
const QString NOTE          = "_Note";
const QString ICON          = "_Icon";

const QString RECORD_ITEM_CONTENT(
        MILLON_SECS + "," +
        DATE_TIME   + "," +
        YEAR        + "," +
        MONTH       + "," +
        DAY         + "," +
        TYPE        + "," +
        PARENT_TYPE + "," +
        CHILD_TYPE  + "," +
        AMOUNT      + "," +
        NOTE        + "," +
        ICON);


// for every type item
const QString ID            = "_ID";
//const QString TYPE          = "_Type";
const QString TYPE_NAME     = "_Type_Name";
const QString INDEX         = "_Index";
//const QString MILLON_SECS   = "_MillonSecs";
//const QString ICON          = "_Icon";
const QString PARENT_ID     = "_Parent_ID";

const QString TYPE_ITEM_CONTENT(
        ID          + "," +
        TYPE        + "," +
        TYPE_NAME   + "," +
        INDEX       + "," +
        MILLON_SECS + "," +
        ICON        + "," +
        PARENT_ID);


const QString DATABASE_BASE_NAME("keep_accounts");
const QString DATABASE_NAME = DATABASE_BASE_NAME + ".db";
const QString DATABASE_DIR_NAME("database");
const QString DATABASE_TABLE_NAME_RECORDS("T_RECORDS");
const QString DATABASE_TABLE_NAME_TYPE("T_TYPE");

const QString TABLE_RECORDS("CREATE TABLE " + DATABASE_TABLE_NAME_RECORDS + "("
                            + MILLON_SECS   + " TEXT NOT NULL,"
                            + DATE_TIME     + " TEXT NOT NULL,"
                            + YEAR          + " TEXT NOT NULL,"
                            + MONTH         + " TEXT NOT NULL,"
                            + DAY           + " TEXT NOT NULL,"
                            + TYPE          + " TEXT NOT NULL,"
                            + PARENT_TYPE   + " TEXT NOT NULL,"
                            + CHILD_TYPE    + " TEXT NOT NULL,"
                            + AMOUNT        + " TEXT NOT NULL,"
                            + NOTE          + " TEXT,"
                            + ICON          + " TEXT)");

const QString TABLE_RECORDS_INSERT("insert into "
                                   + DATABASE_TABLE_NAME_TYPE + " ("
                                   + MILLON_SECS    + ","
                                   + DATE_TIME      + ","
                                   + YEAR           + ","
                                   + MONTH          + ","
                                   + DAY            + ","
                                   + TYPE           + ","
                                   + PARENT_TYPE    + ","
                                   + CHILD_TYPE     + ","
                                   + AMOUNT         + ","
                                   + NOTE           + ","
                                   + ICON           + ") "
                                   + "values(?,?,?,?,?,?,?,?,?,?,?)");

const QString TABLE_TYPE("CREATE TABLE " + DATABASE_TABLE_NAME_TYPE + "("
                         + ID           + " TEXT NOT NULL,"
                         + TYPE         + " TEXT NOT NULL,"
                         + TYPE_NAME    + " TEXT NOT NULL,"
                         + INDEX        + " TEXT NOT NULL,"
                         + MILLON_SECS  + " TEXT NOT NULL,"
                         + ICON         + " TEXT,"
                         + PARENT_ID    + " TEXT)");

}

#endif // CONFIGINFO_H_986BE50A_F753_527F_9CD6_154434F55771
