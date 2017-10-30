/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/30
    File: TypeManager.h
 ***************************************************************************/
#ifndef TYPEMODEL_H_8715A1C4_9597_5E01_B0CC_B7623B3F8263
#define TYPEMODEL_H_8715A1C4_9597_5E01_B0CC_B7623B3F8263

#include <QAbstractListModel>

class TypeModelPrivate;
class TypeItem;

class TypeModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum AnimalRoles {
        TypeIdRole = Qt::UserRole + 1,
        TypeRole,
        TypeNameRole,
        IndexRole,
        MillonSecsRole,
        IconRole,
        ParentIdRole
    };

    TypeModel(QObject *parent = nullptr);
    ~TypeModel();

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

public slots:
    void addType(const TypeItem& typeItem);

private:
    TypeModelPrivate *d;
};

#endif // TYPEMODEL_H_8715A1C4_9597_5E01_B0CC_B7623B3F8263
