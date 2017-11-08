#ifndef INFOMANAGER_H_01925230_009A_5E27_B5AB_642EE461A96C
#define INFOMANAGER_H_01925230_009A_5E27_B5AB_642EE461A96C

#include <QObject>

#include "TypeModel.h"

class InfoManagerPrivate;

class InfoManager : public QObject
{
    Q_OBJECT
public:
    explicit InfoManager(QObject *parent = nullptr);
    ~InfoManager();

    Q_INVOKABLE void initData();
    Q_INVOKABLE TypeModel* typeModel();

private:
    void initTypeData();

private:
    QScopedPointer<InfoManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(InfoManager)
};

#endif // INFOMANAGER_H_01925230_009A_5E27_B5AB_642EE461A96C
