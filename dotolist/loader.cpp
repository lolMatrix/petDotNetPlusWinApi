#include "loader.h"

#include <Event.h>
#include <QObjectList>
#include <QRadioButton>


bool Loader::setImportance(Event *event, QObjectList list)
{
    bool hasChecked = false;
    for(int i = 0; i < list.length(); i++){
        if(list.value(i)->isWidgetType()){
            QRadioButton *button = (QRadioButton *)list.value(i);
            if(button->isChecked()){
                hasChecked = true;
                switch(i){
                    case 1:
                        event->eventImportance = Event::noImportant;
                        break;
                    case 2:
                        event->eventImportance = Event::important;
                        break;
                    case 3:
                        event->eventImportance = Event::veryImportant;
                        break;
                }
            }
        }
    }

    return hasChecked;
}

void Loader::setDurationEvent(Event *event, QTime time)
{
    event->duration = time;
}

void Loader::setDateAndTimeEvent(Event *event, QDateTime date)
{
    event->setEventDate(date);
}

void Loader::setPeriodFlag(Event *event, bool flag)
{
    event->period.hasRepeat = flag;
}

void Loader::setRepeatCount(Event *event, int count)
{
    event->period.repeatCount = count;
}

void Loader::setEventName(Event *event, QString name)
{
    event->name = name;
}

void Loader::setLocation(Event *event, QString location)
{
    event->location = location;
}

bool Loader::setPeriod(Event *event, QObjectList list)
{
    bool hasChecked = false;
    for(int i = 0; i < list.length(); i++){
        if(list.value(i)->isWidgetType()){
            QRadioButton *button = (QRadioButton *)list.value(i);
            if(button->isChecked()){
                hasChecked = true;
                switch(i){
                case 1:
                        event->period.periodEvent = event->period.everyDay;
                    break;
                case 2:
                        event->period.periodEvent = event->period.acrossDay;
                    break;
                case 3:
                        event->period.periodEvent = event->period.acrossTwoDays;
                    break;
                case 4:
                        event->period.periodEvent = event->period.acrossTreeDays;
                    break;
                case 5:
                    event->period.periodEvent = event->period.everyWeak;
                    break;
                case 6:
                    event->period.periodEvent = event->period.everyMouth;
                    break;
                case 7:
                    event->period.periodEvent = event->period.everyQuarter;
                    break;
                case 8:
                    event->period.periodEvent = event->period.everyHalfYear;
                    break;
                case 9:
                    event->period.periodEvent = event->period.everyYear;
                    break;

                }
            }
        }
    }
    return hasChecked;
}

