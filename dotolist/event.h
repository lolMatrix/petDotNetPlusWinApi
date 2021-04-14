#ifndef EVENT_H
#define EVENT_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>



class Event
{
    private:
        QDateTime dateAndTimeEvent = QDateTime::currentDateTime();
        QDateTime correctDate(QDateTime &dateAndTimeEvent);

    public:
        int id;
        QString name;
        QTime duration;
        QString location;

        enum Importance
        {
            noImportant,
            important,
            veryImportant
        }
        eventImportance = noImportant;

        struct Period
        {
            bool hasRepeat = false;

            enum PeriodEvent
            {
                everyDay,
                acrossDay,
                acrossTwoDays,
                acrossTreeDays,
                everyWeak,
                everyMouth,
                everyQuarter,
                everyHalfYear,
                everyYear
            }
            periodEvent = everyDay;

            int repeatCount = 2;
        }
        period;

        QDateTime getEventDate();
        void setEventDate(QDateTime date);
        QString getImportanceString(Importance i);

        bool operator>=(Event right);
        bool operator<=(Event right);
        bool operator>(Event right);
        bool operator<(Event right);
        bool operator!=(Event right);
        bool operator==(Event right);
        Event& operator=(Event right);

        Event();

        QJsonObject Serialize();
    public:
        static Event* Deserialize(QJsonObject object, int id);
};

    #endif // EVENT_H
