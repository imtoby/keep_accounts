#ifndef DBMANAGER_H_5DF607D2_DA87_4954_A19E_8F541D2643E0
#define DBMANAGER_H_5DF607D2_DA87_4954_A19E_8F541D2643E0

#include <QObject>

class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DBMANAGER_H_5DF607D2_DA87_4954_A19E_8F541D2643E0
