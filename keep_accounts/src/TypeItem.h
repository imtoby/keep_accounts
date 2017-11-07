/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/27
    File: TypeItem.h
 ***************************************************************************/
#ifndef TYPEITEM_H_215061BD_CA0C_5FEC_B30B_25A56844F133
#define TYPEITEM_H_215061BD_CA0C_5FEC_B30B_25A56844F133

#include "ConfigInfo.h"

//#include <QObject>

//class TypeItemPrivate;

//class TypeItem : public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(QString typeId READ typeId WRITE setTypeId NOTIFY typeIdChanged)
//    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
//    Q_PROPERTY(QString typeName READ typeName WRITE setTypeName NOTIFY typeNameChanged)
//    Q_PROPERTY(QString index READ index WRITE setIndex NOTIFY indexChanged)
//    Q_PROPERTY(quint64 millonSecs READ millonSecs WRITE setMillonSecs NOTIFY millonSecsChanged)
//    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
//    Q_PROPERTY(QString parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)

//public:
//    explicit TypeItem(QObject *parent = nullptr);
//    ~TypeItem();

//    QString typeId() const;
//    int type() const;
//    QString typeName() const;
//    QString index() const;
//    quint64 millonSecs() const;
//    QString icon() const;
//    QString parentId() const;

//signals:
//    void typeIdChanged(const QString& typeId);
//    void typeChanged(int type);
//    void typeNameChanged(const QString& typeName);
//    void indexChanged(const QString& index);
//    void millonSecsChanged(quint64 millonSecs);
//    void iconChanged(const QString& icon);
//    void parentIdChanged(const QString& parentId);

//public slots:
//    void setTypeId(const QString& typeId);
//    void setType(int type);
//    void setTypeName(const QString& typeName);
//    void setIndex(const QString& index);
//    void setMillonSecs(quint64 millonSecs);
//    void setIcon(const QString& icon);
//    void setParentId(const QString& parentId);

//private:
//    TypeItemPrivate* d;
//};

class TypeItem
{
public:
    QString     typeId;
    KA::InorOut type;
    QString     typeName;
    QString     index;
    quint64     millonSecs;
    QString     icon;
    QString     parentId;
};

#endif // TYPEITEM_H_215061BD_CA0C_5FEC_B30B_25A56844F133
