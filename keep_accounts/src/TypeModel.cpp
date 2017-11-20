#include "TypeModel.h"
#include "ConfigInfo.h"

#include "base/kglobal.h"

class TypeModelPrivate
{
public:
    TypeModelPrivate(TypeModel* parent)
        : q_ptr(parent)
        , type(KA::OUT)
    {}

private:
    TypeModel * const q_ptr;
    Q_DECLARE_PUBLIC(TypeModel)

    KA::InorOut type;
};

TypeModel::TypeModel(QObject* parent)
    : ObjectModel(parent)
    , d_ptr(new TypeModelPrivate(this))
{
}

TypeModel::~TypeModel()
{
}

int TypeModel::type() const
{
    C_D(TypeModel);
    return d->type;
}

void TypeModel::setType(int type)
{
    Q_D(TypeModel);
    if (type != d->type) {
        d->type = KA::InorOut(type);
        emit typeChanged();
    }
}

void TypeModel::appendList(QObjectList *objectList)
{
    for (QObject* object : *objectList) {
        append(object);
    }
}

TypeInfo *TypeModel::getTypeInfoByTypeId(const QString &typeId)
{
    foreach (QObject* object, *this->getAll()) {
        TypeInfo* info = qobject_cast<TypeInfo*>(object);
        if (info->typeId() == typeId) {
            return info;
        }
    }
    return NULL;
}
