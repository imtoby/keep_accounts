/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/11/24
    File: RecordModel.h
 ***************************************************************************/
#ifndef RECORDMODEL_H_E5D7AB71_36FD_545E_8CC7_571AE397E4F7
#define RECORDMODEL_H_E5D7AB71_36FD_545E_8CC7_571AE397E4F7

#include "base/ObjectModel.h"

class RecordModelPrivate;

class RecordModel : public ObjectModel
{
    Q_OBJECT
public:
    explicit RecordModel(QObject* parent = nullptr);
    ~RecordModel();

    void deleteRecord(quint64 millonSecs);

private:
    QScopedPointer<RecordModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(RecordModel)
};

#endif // RECORDMODEL_H_E5D7AB71_36FD_545E_8CC7_571AE397E4F7
