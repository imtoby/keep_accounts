#include "RecordItem.h"
#include "ConfigInfo.h"
#include "base/kglobal.h"

#include <QDateTime>

class RecordItemPrivate
{
public:
    RecordItemPrivate(RecordItem *parent)
        : q_ptr(parent)
        , type(KA::OUT)
    {
        dateTime = QDateTime::currentDateTime().toString(KA::DATE_TIME_FORMAT);
        millonSecs = QDateTime::currentMSecsSinceEpoch();
        QDate date = QDate::currentDate();
        year    = date.year();
        month   = date.month();
        day     = date.day();
    }

private:
    RecordItem * const q_ptr;
    Q_DECLARE_PUBLIC(RecordItem)

    QString     dateTime;
    quint64     millonSecs;
    int         year;
    int         month;
    int         day;
    KA::InorOut type;
    QString     parentType;
    QString     childType;
    QString     note;
    double      amount;
    QString     icon;
};

RecordItem::RecordItem(QObject *parent)
    : QObject(parent)
    , d_ptr(new RecordItemPrivate(this))
{
}

RecordItem::~RecordItem()
{
}

QString RecordItem::dateTime() const
{
    C_D(RecordItem);
    return d->dateTime;
}

quint64 RecordItem::millonSecs() const
{
    C_D(RecordItem);
    return d->millonSecs;
}

int RecordItem::year() const
{
    C_D(RecordItem);
    return d->year;
}

int RecordItem::month() const
{
    C_D(RecordItem);
    return d->month;
}

int RecordItem::day() const
{
    C_D(RecordItem);
    return d->day;
}

int RecordItem::type() const
{
    C_D(RecordItem);
    return d->type;
}

QString RecordItem::parentType() const
{
    C_D(RecordItem);
    return d->parentType;
}

QString RecordItem::childType() const
{
    C_D(RecordItem);
    return d->childType;
}

QString RecordItem::note() const
{
    C_D(RecordItem);
    return d->note;
}

double RecordItem::amount() const
{
    C_D(RecordItem);
    return d->amount;
}

QString RecordItem::icon() const
{
    C_D(RecordItem);
    return d->icon;
}

void RecordItem::setDateTime(const QString &dateTime)
{
    Q_D(RecordItem);
    if (d->dateTime != dateTime) {
        d->dateTime = dateTime;
        emit dateTimeChanged(d->dateTime);
    }
}

void RecordItem::setMillonSecs(quint64 millonSecs)
{
    Q_D(RecordItem);
    if (d->millonSecs != millonSecs) {
        d->millonSecs = millonSecs;
        emit millonSecsChanged(d->millonSecs);
    }
}

void RecordItem::setYear(int year)
{
    Q_D(RecordItem);
    if (d->year != year) {
        d->year = year;
        emit yearChanged(d->year);
    }
}

void RecordItem::setMonth(int month)
{
    Q_D(RecordItem);
    if (d->month != month) {
        d->month = month;
        emit monthChanged(d->month);
    }
}

void RecordItem::setDay(int day)
{
    Q_D(RecordItem);
    if (d->day != day) {
        d->day = day;
        emit dayChanged(d->day);
    }
}

void RecordItem::setType(int type)
{
    Q_D(RecordItem);
    if (d->type != type && type >= KA::OUT && type <= KA::IN) {
        d->type = (KA::InorOut)type;
        emit typeChanged(d->type);
    }
}

void RecordItem::setParentType(const QString &parentType)
{
    Q_D(RecordItem);
    if (d->parentType != parentType) {
        d->parentType = parentType;
        emit parentTypeChanged(d->parentType);
    }
}

void RecordItem::setChildType(const QString &childType)
{
    Q_D(RecordItem);
    if (d->childType != childType) {
        d->childType = childType;
        emit childTypeChanged(d->childType);
    }
}

void RecordItem::setNote(const QString &note)
{
    Q_D(RecordItem);
    if (d->note != note) {
        d->note = note;
        emit noteChanged(d->note);
    }
}

void RecordItem::setAmount(double amount)
{
    Q_D(RecordItem);
    if (d->amount != amount) {
        d->amount = amount;
        emit amountChanged(d->amount);
    }
}

void RecordItem::setIcon(const QString &icon)
{
    Q_D(RecordItem);
    if (d->icon != icon) {
        d->icon = icon;
        emit iconChanged(d->icon);
    }
}
