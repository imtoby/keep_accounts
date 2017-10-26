#include "DBManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "ConfigInfo.h"

namespace {

QString DatabaseDirPath = "";
QString DatabaseFileName = "";

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
