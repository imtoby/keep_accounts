#include "TypeInfo.h"

#include "ConfigInfo.h"
#include "base/kglobal.h"

class TypeInfoPrivate
{
public:
    TypeInfoPrivate(TypeInfo *parent)
        : q_ptr(parent)
        , typeId(QString())
        , type(KA::OUT)
        , typeName(QString())
        , index(QString())
        , millonSecs(0)
        , icon(QString())
        , parentId(QString())
    {}

    void init();
    void uninit();

private:
    TypeInfo * const q_ptr;
    Q_DECLARE_PUBLIC(TypeInfo)

    QString     typeId;
    KA::InorOut type;
    QString     typeName;
    QString     index;
    quint64     millonSecs;
    QString     icon;
    QString     parentId;
};

TypeInfo::TypeInfo(QObject *parent)
    : QObject(parent)
    , d_ptr(new TypeInfoPrivate(this))
{
    Q_D(TypeInfo);
    d->init();
}

TypeInfo::~TypeInfo()
{
    Q_D(TypeInfo);
    d->uninit();
}

QString TypeInfo::typeId() const
{
    C_D(TypeInfo);
    return d->typeId;
}

int TypeInfo::type() const
{
    C_D(TypeInfo);
    return d->type;
}

QString TypeInfo::typeName() const
{
    C_D(TypeInfo);
    return d->typeName;
}

QString TypeInfo::index() const
{
    C_D(TypeInfo);
    return d->index;
}

quint64 TypeInfo::millonSecs() const
{
    C_D(TypeInfo);
    return d->millonSecs;
}

QString TypeInfo::icon() const
{
    C_D(TypeInfo);
    return d->icon;
}

QString TypeInfo::parentId() const
{
    C_D(TypeInfo);
    return d->parentId;
}

void TypeInfo::setTypeId(const QString &typeId)
{
    Q_D(TypeInfo);
    if (d->typeId != typeId) {
        d->typeId = typeId;
        emit typeIdChanged(d->typeId);
    }
}

void TypeInfo::setType(int type)
{
    Q_D(TypeInfo);
    if (d->type != type && type >= KA::OUT && type <= KA::IN) {
        d->type = (KA::InorOut)type;
        emit typeChanged(d->type);
    }
}

void TypeInfo::setTypeName(const QString &typeName)
{
    Q_D(TypeInfo);
    if (d->typeName != typeName) {
        d->typeName = typeName;
        emit typeNameChanged(d->typeName);
    }
}

void TypeInfo::setIndex(const QString &index)
{
    Q_D(TypeInfo);
    if (d->index != index) {
        d->index = index;
        emit indexChanged(d->index);
    }
}

void TypeInfo::setMillonSecs(quint64 millonSecs)
{
    Q_D(TypeInfo);
    if (d->millonSecs != millonSecs) {
        d->millonSecs = millonSecs;
        emit millonSecsChanged(d->millonSecs);
    }
}

void TypeInfo::setIcon(const QString &icon)
{
    Q_D(TypeInfo);
    if (d->icon != icon) {
        d->icon = icon;
        emit iconChanged(d->icon);
    }
}

void TypeInfo::setParentId(const QString &parentId)
{
    Q_D(TypeInfo);
    if (d->parentId != parentId) {
        d->parentId = parentId;
        emit parentIdChanged(d->parentId);
    }
}

void TypeInfoPrivate::init()
{
}

void TypeInfoPrivate::uninit()
{
}
