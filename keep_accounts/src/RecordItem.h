/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/26
    File: RecordItem.h
 ***************************************************************************/
#ifndef RECORDITEM_H_3E7BB3B8_B3E8_51AA_A1FF_BB92B02DA72E
#define RECORDITEM_H_3E7BB3B8_B3E8_51AA_A1FF_BB92B02DA72E

#include <QObject>

class RecordItemPrivate;

class RecordItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(quint64 millonSecs READ millonSecs WRITE setMillonSecs NOTIFY millonSecsChanged)
    Q_PROPERTY(int year READ year WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(int month READ month WRITE setMonth NOTIFY monthChanged)
    Q_PROPERTY(int day READ day WRITE setDay NOTIFY dayChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString parentType READ parentType WRITE setParentType NOTIFY parentTypeChanged)
    Q_PROPERTY(QString childType READ childType WRITE setChildType NOTIFY childTypeChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
    Q_PROPERTY(double amount READ amount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)

public:
    explicit RecordItem(QObject *parent = nullptr);
    ~RecordItem();

    QString dateTime() const;
    quint64 millonSecs() const;
    int year() const;
    int month() const;
    int day() const;
    int type() const;
    QString parentType() const;
    QString childType() const;
    QString note() const;
    double amount() const;
    QString icon() const;

signals:
    void dateTimeChanged(const QString& dateTime);
    void millonSecsChanged(quint64 millonSecs);
    void yearChanged(int year);
    void monthChanged(int month);
    void dayChanged(int day);
    void typeChanged(int type);
    void parentTypeChanged(const QString& parentType);
    void childTypeChanged(const QString& childType);
    void noteChanged(const QString& note);
    void amountChanged(double amount);
    void iconChanged(const QString& icon);

public slots:
    void setDateTime(const QString& dateTime);
    void setMillonSecs(quint64 millonSecs);
    void setYear(int year);
    void setMonth(int month);
    void setDay(int day);
    void setType(int type);
    void setParentType(const QString& parentType);
    void setChildType(const QString& childType);
    void setNote(const QString& note);
    void setAmount(double amount);
    void setIcon(const QString& icon);

private:
    RecordItemPrivate *d;
};

#endif // RECORDITEM_H_3E7BB3B8_B3E8_51AA_A1FF_BB92B02DA72E
