/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/11/24
    File: RecordModel.h
 ***************************************************************************/
#include "RecordModel.h"
#include "RecordItem.h"

class RecordModelPrivate
{
public:
    RecordModelPrivate(RecordModel *parent)
        : q_ptr(parent)
    {}

private:
    RecordModel * const q_ptr;
    Q_DECLARE_PUBLIC(RecordModel)
};

RecordModel::RecordModel(QObject *parent)
    : ObjectModel(parent)
    , d_ptr(new RecordModelPrivate(this))
{
}

RecordModel::~RecordModel()
{
}

void RecordModel::deleteRecord(quint64 millonSecs)
{
    QObjectList* list = getAll();
    for (int i=0; i<list->size(); ++i) {
        const RecordItem* const item = qobject_cast<RecordItem*>(list->at(i));
        if (item->millonSecs() == millonSecs) {
            remove(i);
            break;
        }
    }
}
