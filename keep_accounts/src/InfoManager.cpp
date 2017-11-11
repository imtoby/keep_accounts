#include "InfoManager.h"
#include "DBManager.h"
#include "TypeInfo.h"

class InfoManagerPrivate
{
public:
    InfoManagerPrivate(InfoManager *parent)
        : q_ptr(parent)
        , topInModel(NULL)
        , childInModel(NULL)
        , topOutModel(NULL)
        , childOutModel(NULL)
    {}

    void init();
    void uninit();

private:
    InfoManager * const q_ptr;
    Q_DECLARE_PUBLIC(InfoManager)

    TypeModel* topInModel;
    TypeModel* childInModel;
    TypeModel* topOutModel;
    TypeModel* childOutModel;
};

InfoManager::InfoManager(QObject *parent)
    : QObject(parent)
    , d_ptr(new InfoManagerPrivate(this))
{
    Q_D(InfoManager);
    d->init();
    initData();
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

    if (KA::OUT == type) {
        if (parentId == KA::TOP_TYPE_ID) {
            return d->topOutModel;
        } else {
            QObjectList infos = KA_DB->getTypeInfos(KA::OUT, this, parentId);
            d->childOutModel->clear();
            d->childOutModel->set(&infos);
            return d->childOutModel;
        }
    } else {
        if (parentId == KA::TOP_TYPE_ID) {
            return d->topInModel;
        } else {
            QObjectList infos = KA_DB->getTypeInfos(KA::IN, this, parentId);
            d->childInModel->clear();
            d->childInModel->set(&infos);
            return d->childInModel;
        }
    }

    return d->topOutModel;
}

void InfoManager::initTypeData()
{
    Q_D(InfoManager);
    QObjectList infos = KA_DB->getTypeInfos(KA::IN, this, KA::TOP_TYPE_ID);
    d->topInModel->clear();
    d->topInModel->set(&infos);

    infos = KA_DB->getTypeInfos(KA::OUT, this, KA::TOP_TYPE_ID);
    d->topOutModel->clear();
    d->topOutModel->set(&infos);

    emit initTypeFinished();
}

void InfoManagerPrivate::init()
{
    Q_Q(InfoManager);
    if (topInModel == NULL) {
        topInModel = new TypeModel(q);
    }
    if (childInModel == NULL) {
        childInModel = new TypeModel(q);
    }
    if (topOutModel == NULL) {
        topOutModel = new TypeModel(q);
    }
    if (childOutModel == NULL) {
        childOutModel = new TypeModel(q);
    }
}

void InfoManagerPrivate::uninit()
{
    if (topInModel) {
        topInModel->deleteLater();
        topInModel = NULL;
    }
    if (childInModel) {
        childInModel->deleteLater();
        childInModel = NULL;
    }
    if (topOutModel) {
        topOutModel->deleteLater();
        topOutModel = NULL;
    }
    if (childOutModel) {
        childOutModel->deleteLater();
        childOutModel = NULL;
    }
}
