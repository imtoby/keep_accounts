#ifndef INFOMANAGER_H_01925230_009A_5E27_B5AB_642EE461A96C
#define INFOMANAGER_H_01925230_009A_5E27_B5AB_642EE461A96C

#include <QObject>

#include "TypeModel.h"
#include "ConfigInfo.h"
#include "RecordItem.h"

class InfoManagerPrivate;

class Worker : public QObject
{
    Q_OBJECT

public slots:
    void doAddType(TypeInfo* info, const QString& typeName,
                   int type, const QString& parentId,
                   const QString& icon = QString());
    void doDeleteType(int index, int type, const QString& typeId,
                      const QString& parentId);

    // for record items
    void doAddRecord(RecordItem* item, int type, const QString& parentId
                     , const QString& typeId, const QString& dateTime
                     , double amount, const QString& note = QString()
            , const QString& icon = QString());

signals:
    void addType(TypeInfo* info, const QString& typeName,
                 int type, const QString& parentId,
                 const QString& icon = QString());
    void addTypeFinished(TypeInfo* info, int type,
                         const QString& parentId);
    void deleteTypeFinished(int index, int type, const QString& typeId,
                            const QString& parentId);

    // for record items
    void addRecord(RecordItem* item, int type, const QString& parentId
                   , const QString& typeId, const QString& dateTime
                   , double amount, const QString& note = QString()
          , const QString& icon = QString());
    void addRecordFinished(RecordItem* item, int type, const QString& parentId);
};

class InfoManager : public QObject
{
    Q_OBJECT
public:
    explicit InfoManager(QObject *parent = nullptr);
    ~InfoManager();

    Q_INVOKABLE QString dateFormat() const {
        return KA::DATE_FORMAT;
    }

    Q_INVOKABLE QString topTypeId() const {
        return KA::TOP_TYPE_ID;
    }

    Q_INVOKABLE void initData();
    Q_INVOKABLE TypeModel* editTypeModel(int type,
                                         const QString& parentId
                                         = KA::TOP_TYPE_ID);

    Q_INVOKABLE TypeModel* selectTypeModel(int type);

    Q_INVOKABLE void setTypeName(int type, const QString& typeId,
                                 const QString& parentId,
                                 const QString& typeName);

private:
    // parentId could not equals KA::TOP_TYPE_ID
    QString getParentTypeName(int type, const QString& parentId) const;

signals:
    // for type infos
    void initTypeFinished();
    void addType(const QString& typeName, int type, const QString& parentId,
                 const QString& icon = QString());
    void addTypeFinished(int type, const QString& parentId, int size);
    void deleteType(int index, int type, const QString& typeId,
                    const QString& parentId);
    void deleteTypeFinished();

    // for record items
    void addRecord(int type, const QString& parentId, const QString& typeId
                   , const QString& dateTime, double amount
                   , const QString& note = QString()
            , const QString& icon = QString());

private slots:
    void initTypeData();
    void doAddType(const QString& typeName, int type, const QString& parentId,
                   const QString& icon = QString());
    void doAddTypeFinished(TypeInfo* info, int type,
                           const QString& parentId);
    void doDeleteTypeFinished(int index, int type, const QString& typeId,
                              const QString& parentId);

    // for record items
    void initRecordData();
    void doAddRecord(int type, const QString& parentId, const QString& typeId
                     , const QString& dateTime, double amount
                     , const QString& note = QString()
            , const QString& icon = QString());
    void doAddRecordFinished(RecordItem* item, int type,
                             const QString& parentId);


private:
    QScopedPointer<InfoManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(InfoManager)
};

#endif // INFOMANAGER_H_01925230_009A_5E27_B5AB_642EE461A96C
