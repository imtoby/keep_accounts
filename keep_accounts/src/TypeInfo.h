/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/11/08
    File: TypeInfo.h
 ***************************************************************************/
#ifndef TYPEINFO_H_4E2348D0_B658_531B_A296_0B470AAFD474
#define TYPEINFO_H_4E2348D0_B658_531B_A296_0B470AAFD474

#include <QObject>

class TypeInfoPrivate;

class TypeInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString typeId READ typeId WRITE setTypeId NOTIFY typeIdChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString typeName READ typeName WRITE setTypeName NOTIFY typeNameChanged)
    Q_PROPERTY(QString index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(quint64 millonSecs READ millonSecs WRITE setMillonSecs NOTIFY millonSecsChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)
public:
    explicit TypeInfo(QObject *parent = nullptr);
    ~TypeInfo();

    QString typeId() const;
    int type() const;
    QString typeName() const;
    QString index() const;
    quint64 millonSecs() const;
    QString icon() const;
    QString parentId() const;

    void setTypeId(const QString& typeId);
    void setType(int type);
    void setTypeName(const QString& typeName);
    void setIndex(const QString& index);
    void setMillonSecs(quint64 millonSecs);
    void setIcon(const QString& icon);
    void setParentId(const QString& parentId);

signals:
    void typeIdChanged(const QString& typeId);
    void typeChanged(int type);
    void typeNameChanged(const QString& typeName);
    void indexChanged(const QString& index);
    void millonSecsChanged(quint64 millonSecs);
    void iconChanged(const QString& icon);
    void parentIdChanged(const QString& parentId);

private:
    QScopedPointer<TypeInfoPrivate> d_ptr;
    Q_DECLARE_PRIVATE(TypeInfo)
};

#endif // TYPEINFO_H_4E2348D0_B658_531B_A296_0B470AAFD474
