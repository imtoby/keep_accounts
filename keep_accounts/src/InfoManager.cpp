#include "InfoManager.h"
#include "DBManager.h"

class InfoManagerPrivate
{
public:
    InfoManagerPrivate(InfoManager *parent)
        : q_ptr(parent)
        , typeModel(NULL)
    {}

    void init();
    void uninit();

private:
    InfoManager * const q_ptr;
    Q_DECLARE_PUBLIC(InfoManager)

    TypeModel* typeModel;
};

InfoManager::InfoManager(QObject *parent)
    : QObject(parent)
    , d_ptr(new InfoManagerPrivate(this))
{
    Q_D(InfoManager);
    d->init();
}

InfoManager::~InfoManager()
{
    Q_D(InfoManager);
    d->uninit();
}

void InfoManager::initData()
{
    initTypeData();
}

TypeModel *InfoManager::typeModel(int type, const QString& parentId)
{
    Q_D(InfoManager);

    QObjectList infos = KA_DB->getTypeInfos(KA::InorOut(type), parentId);
    d->typeModel->clear();
    d->typeModel->set(&infos);

    return d->typeModel;
}

void InfoManager::initTypeData()
{
}

void InfoManagerPrivate::init()
{
    Q_Q(InfoManager);
    if (typeModel == NULL) {
        typeModel = new TypeModel(q);
    }
}

void InfoManagerPrivate::uninit()
{
    if (typeModel) {
        typeModel->deleteLater();
        typeModel = NULL;
    }
}
