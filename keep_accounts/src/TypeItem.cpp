#include "TypeItem.h"

//#include "ConfigInfo.h"

//class TypeItemPrivate
//{
//public:
//    TypeItemPrivate()
//        : type(KA::OUT)
//    {}

//    QString     typeId;
//    KA::InorOut type;
//    QString     typeName;
//    QString     index;
//    quint64     millonSecs;
//    QString     icon;
//    QString     parentId;
//};

//TypeItem::TypeItem(QObject *parent)
//    : QObject(parent)
//    , d(new TypeItemPrivate)
//{

//}

//TypeItem::~TypeItem()
//{
//    if (d) {
//        delete d;
//        d = 0;
//    }
//}

//QString TypeItem::typeId() const
//{
//    return d->typeId;
//}

//int TypeItem::type() const
//{
//    return d->type;
//}

//QString TypeItem::typeName() const
//{
//    return d->typeName;
//}

//QString TypeItem::index() const
//{
//    return d->index;
//}

//quint64 TypeItem::millonSecs() const
//{
//    return d->millonSecs;
//}

//QString TypeItem::icon() const
//{
//    return d->icon;
//}

//QString TypeItem::parentId() const
//{
//    return d->parentId;
//}

//void TypeItem::setTypeId(const QString &typeId)
//{
//    if (d->typeId != typeId) {
//        d->typeId = typeId;
//        emit typeIdChanged(d->typeId);
//    }
//}

//void TypeItem::setType(int type)
//{
//    if (d->type != type && type >= KA::OUT && type <= KA::IN) {
//        d->type = (KA::InorOut)type;
//        emit typeChanged(d->type);
//    }
//}

//void TypeItem::setTypeName(const QString &typeName)
//{
//    if (d->typeName != typeName) {
//        d->typeName = typeName;
//        emit typeNameChanged(d->typeName);
//    }
//}

//void TypeItem::setIndex(const QString &index)
//{
//    if (d->index != index) {
//        d->index = index;
//        emit indexChanged(d->index);
//    }
//}

//void TypeItem::setMillonSecs(quint64 millonSecs)
//{
//    if (d->millonSecs != millonSecs) {
//        d->millonSecs = millonSecs;
//        emit millonSecsChanged(d->millonSecs);
//    }
//}

//void TypeItem::setIcon(const QString &icon)
//{
//    if (d->icon != icon) {
//        d->icon = icon;
//        emit iconChanged(d->icon);
//    }
//}

//void TypeItem::setParentId(const QString &parentId)
//{
//    if (d->parentId != parentId) {
//        d->parentId = parentId;
//        emit parentIdChanged(d->parentId);
//    }
//}
