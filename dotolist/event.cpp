#include "event.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>

bool Event::operator>=(Event right)
{
    bool result = false;
    if (name == right.name){
        result = getEventDate() >= right.getEventDate();
    }
    else {
        result = name >= right.name;
    }
    return result;
}

bool Event::operator<=(Event right)
{
    bool result = false;
    if (name == right.name){
        result = getEventDate() <= right.getEventDate();
    }
    else {
        result = name <= right.name;
    }
    return result;
}

bool Event::operator>(Event right)
{
    bool result = false;
    if (dateAndTimeEvent == right.dateAndTimeEvent){
        result = name > right.name;
    }
    else {
        result = getEventDate() > right.getEventDate();
    }
    return result;
}

bool Event::operator<(Event right)
{
    bool result = false;
    if (name == right.name){
        result = getEventDate() < right.getEventDate();
    }
    else {
        result = name < right.name;
    }
    return result;
}

bool Event::operator!=(Event right)
{
    bool result = false;
    if (name == right.name){
        result = getEventDate() != right.getEventDate();
    }
    else {
        result = true;
    }
    return result;
}

bool Event::operator==(Event right)
{
    bool result = false;
    if (name == right.name){
        result = getEventDate() == right.getEventDate();
    }
    return result;
}

Event& Event::operator=(Event right)
{
    if (this == &right) {
         return *this;
    }
    name = right.name;
    dateAndTimeEvent = right.getEventDate();
    duration = right.duration;
    eventImportance = right.eventImportance;
    period = right.period;
    return *this;
}

Event::Event()
{
    dateAndTimeEvent = QDateTime::currentDateTime();
    dateAndTimeEvent = dateAndTimeEvent.addDays(rand() % 20);
    dateAndTimeEvent.setTime(QTime(dateAndTimeEvent.time().hour(), dateAndTimeEvent.time().minute(), dateAndTimeEvent.time().second()));
    name = "New Event";
    location = "da";
    duration = QTime(1, 0);
    eventImportance = noImportant;
    period.repeatCount = 2;
    period.periodEvent = Period::everyDay;
}

QJsonObject Event::Serialize()
{
    QJsonObject serialized;
    QJsonObject serializedPeriod;

    serialized.insert("Name", name);
    serialized.insert("Date", getEventDate().toSecsSinceEpoch());
    serialized.insert("Duration", QTime(0,0,0).secsTo(duration));
    serialized.insert("Location", location);
    serialized.insert("ImportanceId", (int)eventImportance);

    serializedPeriod.insert("HasPeriod", period.hasRepeat);
    serializedPeriod.insert("PeriodId", (int)period.periodEvent);
    serializedPeriod.insert("RepeatCount", period.repeatCount);

    serialized.insert("period", serializedPeriod);

    return serialized;
}

Event* Event::Deserialize(QJsonObject object, int id)
{
    Event *deserialized = new Event;
    deserialized->id = object.take("Id").toInt();
    deserialized->name = object.take("Name").toString();
    deserialized->setEventDate(QDateTime::fromSecsSinceEpoch(object.take("Date").toInt()));
    deserialized->duration = QTime(0, 0, 0).addSecs(object.take("Duration").toInt());
    deserialized->location = object.take("Location").toString();
    deserialized->eventImportance = (Event::Importance) object.take("ImportanceId").toInt();

    QJsonObject period = object.take("period").toObject();
    deserialized->period.hasRepeat = period.take("HasPeriod").toBool();
    deserialized->period.periodEvent = (Event::Period::PeriodEvent) period.take("PeriodId").toInt();
    deserialized->period.repeatCount = period.take("RepeatCount").toInt();

    return deserialized;
}

QDateTime Event::getEventDate(){
    if (period.hasRepeat)
        dateAndTimeEvent = correctDate(dateAndTimeEvent);
    return dateAndTimeEvent;
}

void Event::setEventDate(QDateTime dateAndTimeEvent){
    if(period.hasRepeat){
        dateAndTimeEvent = correctDate(dateAndTimeEvent);
    }
    this->dateAndTimeEvent = dateAndTimeEvent;
}

QDateTime Event::correctDate(QDateTime &dateAndTimeEvent)
{
    if(QDateTime::currentDateTime() >= dateAndTimeEvent){
        switch(period.periodEvent){
        case period.everyDay:
            dateAndTimeEvent.setDate(QDate::currentDate().addDays(1));
            break;
        case period.acrossDay:
            dateAndTimeEvent.setDate(QDate::currentDate().addMonths(2));
            break;
        case period.acrossTwoDays:
            dateAndTimeEvent.setDate(QDate::currentDate().addDays(3));
            break;
        case period.acrossTreeDays:
            dateAndTimeEvent.setDate(QDate::currentDate().addDays(4));
            break;
        case period.everyWeak:
            dateAndTimeEvent.setDate(QDate::currentDate().addDays(7));
            break;
        case period.everyMouth:
            dateAndTimeEvent.setDate(QDate::currentDate().addMonths(1));
            break;
        case period.everyQuarter:
            dateAndTimeEvent.setDate(QDate::currentDate().addMonths(3));
            break;
        case period.everyHalfYear:
            dateAndTimeEvent.setDate(QDate::currentDate().addMonths(6));
            break;
        case period.everyYear:
            dateAndTimeEvent.setDate(QDate::currentDate().addYears(1));
            break;
        }
    }

        return dateAndTimeEvent;
}

QString Event::getImportanceString(Importance i){
    QString result = "";
    switch (i) {
    case Event::noImportant:
        result = "Не важно";
        break;
    case Event::important:
        result = "Важно";
        break;
    case Event::veryImportant:
        result = "Очень важно";
        break;
    }

    return result;
}


