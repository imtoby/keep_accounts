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
