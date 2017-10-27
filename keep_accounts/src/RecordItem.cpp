#include "RecordItem.h"
#include "ConfigInfo.h"
#include "DBManager.h"

class RecordItemPrivate
{
public:
    RecordItemPrivate()
        : type(KA::OUT)
    {}

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
    , d(new RecordItemPrivate)
{
}

RecordItem::~RecordItem()
{
    if (d) {
        delete d;
        d = 0;
    }
}

QString RecordItem::dateTime() const
{
    return d->dateTime;
}

quint64 RecordItem::millonSecs() const
{
    return d->millonSecs;
}

int RecordItem::year() const
{
    return d->year;
}

int RecordItem::month() const
{
    return d->month;
}

int RecordItem::day() const
{
    return d->day;
}

int RecordItem::type() const
{
    return d->type;
}

QString RecordItem::parentType() const
{
    return d->parentType;
}

QString RecordItem::childType() const
{
    return d->childType;
}

QString RecordItem::note() const
{
    return d->note;
}

double RecordItem::amount() const
{
    return d->amount;
}

QString RecordItem::icon() const
{
    return d->icon;
}

void RecordItem::setDateTime(const QString &dateTime)
{
    if (d->dateTime != dateTime) {
        d->dateTime = dateTime;
        emit dateTimeChanged(d->dateTime);
    }
}

void RecordItem::setMillonSecs(quint64 millonSecs)
{
    if (d->millonSecs != millonSecs) {
        d->millonSecs = millonSecs;
        emit millonSecsChanged(d->millonSecs);
    }
}

void RecordItem::setYear(int year)
{
    if (d->year != year) {
        d->year = year;
        emit yearChanged(d->year);
    }
}

void RecordItem::setMonth(int month)
{
    if (d->month != month) {
        d->month = month;
        emit monthChanged(d->month);
    }
}

void RecordItem::setDay(int day)
{
    if (d->day != day) {
        d->day = day;
        emit dayChanged(d->day);
    }
}

void RecordItem::setType(int type)
{
    if (d->type != type && type >= KA::OUT && type <= KA::IN) {
        d->type = (KA::InorOut)type;
        emit typeChanged(d->type);
    }
}

void RecordItem::setParentType(const QString &parentType)
{
    if (d->parentType != parentType) {
        d->parentType = parentType;
        emit parentTypeChanged(d->parentType);
    }
}

void RecordItem::setChildType(const QString &childType)
{
    if (d->childType != childType) {
        d->childType = childType;
        emit childTypeChanged(d->childType);
    }
}

void RecordItem::setNote(const QString &note)
{
    if (d->note != note) {
        d->note = note;
        emit noteChanged(d->note);
    }
}

void RecordItem::setAmount(double amount)
{
    if (d->amount != amount) {
        d->amount = amount;
        emit amountChanged(d->amount);
    }
}

void RecordItem::setIcon(const QString &icon)
{
    if (d->icon != icon) {
        d->icon = icon;
        emit iconChanged(d->icon);
    }
}
