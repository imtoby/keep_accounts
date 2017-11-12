#ifndef INFOMANAGER_H_01925230_009A_5E27_B5AB_642EE461A96C
#define INFOMANAGER_H_01925230_009A_5E27_B5AB_642EE461A96C

#include <QObject>

#include "TypeModel.h"
#include "ConfigInfo.h"

class InfoManagerPrivate;

class InfoManager : public QObject
{
    Q_OBJECT
public:
    explicit InfoManager(QObject *parent = nullptr);
    ~InfoManager();

    Q_INVOKABLE void initData();
    Q_INVOKABLE TypeModel* typeModel(int type,
                                     const QString& parentId = KA::TOP_TYPE_ID);

    Q_INVOKABLE void setTypeName(int type, const QString& typeId,
                                 const QString& parentId,
                                 const QString& typeName);

    Q_INVOKABLE void addType(const QString& typeName, int type,
                             const QString& parentId,
                             const QString& icon = QString());

signals:
    void initTypeFinished();

private:
    void initTypeData();
    int getCount(int type, const QString& parentId) const;

private:
    QScopedPointer<InfoManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(InfoManager)
};

#endif // INFOMANAGER_H_01925230_009A_5E27_B5AB_642EE461A96C
