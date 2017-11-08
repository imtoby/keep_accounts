#ifndef TYPEMODEL_H_AEB5BAAE_7DBF_531A_A17E_123B9E0D0EA9
#define TYPEMODEL_H_AEB5BAAE_7DBF_531A_A17E_123B9E0D0EA9

#include "base/ObjectModel.h"

class TypeModelPrivate;

class TypeModel : public ObjectModel
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
public:
    explicit TypeModel(QObject* parent = nullptr);
    ~TypeModel();

    int type() const;
    void setType(int type);

signals:
    void typeChanged();

private:
    QScopedPointer<TypeModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(TypeModel)
};

#endif // TYPEMODEL_H_AEB5BAAE_7DBF_531A_A17E_123B9E0D0EA9
