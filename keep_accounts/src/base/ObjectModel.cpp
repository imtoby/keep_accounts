/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/11/07
    File: ObjectModel.cpp
 ***************************************************************************/
#include "ObjectModel.h"
#include <QMutex>
#include <QDebug>

#include "kglobal.h"

namespace {

enum {
    ObjectModelRole = Qt::UserRole + 1
};

}

class ObjectModelPrivate
{
public:
    ObjectModelPrivate(ObjectModel * parent)
        : q_ptr(parent)
        , mutex(NULL)
    {}

    void init();
    void uninit();

private:
    ObjectModel * const q_ptr;
    Q_DECLARE_PUBLIC(ObjectModel)

    QHash<int, QByteArray> rolesNames;
    QObjectList objectList;
    QMutex *mutex;
};

ObjectModel::ObjectModel(QObject *parent) :
    QAbstractListModel(parent),
    d_ptr(new ObjectModelPrivate(this))
{
    Q_D(ObjectModel);
    d->init();
    d->rolesNames[ObjectModelRole] = "objectModelRole";
}

ObjectModel::~ObjectModel()
{
    Q_D(ObjectModel);
    d->uninit();
}

int ObjectModel::rowCount(const QModelIndex &parent) const
{
    C_D(ObjectModel);
    Q_UNUSED(parent);
    return d->objectList.size();
}

int ObjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ObjectModel::data(const QModelIndex &index, int role) const
{
    C_D(ObjectModel);
    if (index.row() >= 0 && index.row() < d->objectList.size() ) {
        if (role == ObjectModelRole) {
            QObject *object = d->objectList.at(index.row());
            return QVariant::fromValue(object);
        }
    }
    return QVariant(0);
}

bool ObjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D(ObjectModel);
    if (index.row() >= 0 && index.row() < d->objectList.size() ) {
        if (role == ObjectModelRole) {
            d->mutex->lock();
            d->objectList.replace(index.row(), value.value<QObject *>());
            d->mutex->unlock();
            return true;
        }
    }
    return false;
}

QHash<int, QByteArray> ObjectModel::roleNames() const
{
    C_D(ObjectModel);
    return d->rolesNames;
}

void ObjectModel::insert(int index, QObject *object)
{
    Q_D(ObjectModel);
    if (index >= 0  && index < d->objectList.size()) {
        d->mutex->lock();
        beginInsertRows(QModelIndex(), index, index);
        d->objectList.insert(index, object);
        endInsertRows();
        d->mutex->unlock();
    }
}

void ObjectModel::append(QObject *object)
{
    Q_D(ObjectModel);
    insert(d->objectList.size(), object);
}

void ObjectModel::push_front(QObject *object)
{
    insert(0, object);
}

void ObjectModel::push_back(QObject *object)
{
    append(object);
}

void ObjectModel::replace(int index, QObject *object)
{
    Q_D(ObjectModel);
    if (index >= 0  && index < d->objectList.size()) {
        d->mutex->lock();
        d->objectList.replace(index, object);
        d->mutex->unlock();
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void ObjectModel::set(QObjectList *objectList)
{
    Q_D(ObjectModel);
    d->mutex->lock();
    beginResetModel();
    d->objectList = *objectList;
    endResetModel();
    d->mutex->unlock();
}

void ObjectModel::remove(QObject *object)
{
    Q_D(ObjectModel);
    d->mutex->lock();
    const int index = d->objectList.indexOf(object);
    if (index >= 0) {
        beginRemoveRows(QModelIndex(), index, index);
        d->objectList.removeAt(index);
        endRemoveRows();
    }
    d->mutex->unlock();
}

void ObjectModel::remove(int index)
{
    Q_D(ObjectModel);
    if (index >= 0 && index < d->objectList.size()) {
        d->mutex->lock();
        beginRemoveRows(QModelIndex(), index, index);
        QObject* object = d->objectList.at(index);
        d->objectList.removeAt(index);
        object->deleteLater();
        endRemoveRows();
        d->mutex->unlock();
    }
}

void ObjectModel::clear()
{
    Q_D(ObjectModel);
    if (d->objectList.size() > 0) {
        d->mutex->lock();
        beginResetModel();
        qDeleteAll(d->objectList);
        d->objectList.clear();
        endResetModel();
        d->mutex->unlock();
    }
}

int ObjectModel::count() const
{
    C_D(ObjectModel);
    return d->objectList.size();
}

QObject *ObjectModel::get(int index)
{
    Q_D(ObjectModel);
    if (index >= 0 && index < d->objectList.size()) {
        d->mutex->lock();
        QObject * object = d->objectList.at(index);
        d->mutex->unlock();
        return object;
    }
    return NULL;
}

QObjectList *ObjectModel::getAll()
{
    Q_D(ObjectModel);
    d->mutex->lock();
    QObjectList * objectList = &(d->objectList);
    d->mutex->unlock();
    return objectList;
}


void ObjectModelPrivate::init()
{
    rolesNames.clear();
    objectList.clear();
    if (mutex == NULL) {
        mutex = new QMutex(QMutex::Recursive);
    }
}

void ObjectModelPrivate::uninit()
{
    qDeleteAll(objectList);
    objectList.clear();
    rolesNames.clear();
    if (mutex) {
        delete mutex;
        mutex = NULL;
    }
}
