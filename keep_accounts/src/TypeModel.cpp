/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/30
    File: TypeManager.h
 ***************************************************************************/

#include "TypeModel.h"
#include <QList>

#include "ConfigInfo.h"
#include "TypeItem.h"

class TypeModelPrivate
{
public:
    TypeModelPrivate() {}

    QList<TypeItem> typesList;
};

TypeModel::TypeModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new TypeModelPrivate)
{

}

TypeModel::~TypeModel()
{
    if (d) {
        delete d;
        d = 0;
    }
}

QVariant TypeModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > d->typesList.count()) {
        return QVariant();
    }
    const TypeItem &typeItem = d->typesList[index.row()];

    switch (role) {
    case TypeIdRole:        return typeItem.typeId;
    case TypeRole:          return typeItem.type;
    case TypeNameRole:      return typeItem.typeName;
    case IndexRole:         return typeItem.index;
    case MillonSecsRole:    return typeItem.millonSecs;
    case IconRole:          return typeItem.icon;
    case ParentIdRole:      return typeItem.parentId;
    default:
        break;
    }
    return QVariant();
}

QHash<int, QByteArray> TypeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeIdRole]       = "typeId";
    roles[TypeRole]         = "type";
    roles[TypeNameRole]     = "typeName";
    roles[IndexRole]        = "typeIndex";
    roles[MillonSecsRole]   = "typeMillonSecs";
    roles[IconRole]         = "typeIcon";
    roles[ParentIdRole]     = "typeParentId";
    return roles;
}

int TypeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d->typesList.count();
}

void TypeModel::addType(const TypeItem &typeItem)
{
    d->typesList.push_back(typeItem);
}
