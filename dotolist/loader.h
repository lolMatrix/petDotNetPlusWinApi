#ifndef LOADER_H
#define LOADER_H

#include <Event.h>
#include <QObjectList>



class Loader
{
public:
    bool setImportance(Event *event, QObjectList list);
    bool setPeriod(Event *event, QObjectList list);
    void setLocation(Event *event, QString location);
    void setEventName(Event *event, QString name);
    void setRepeatCount(Event *event, int count);
    void setPeriodFlag(Event *event, bool flag);
    void setDateAndTimeEvent(Event *event, QDateTime date);
    void setDurationEvent(Event *event, QTime time);
};

#endif // LOADER_H
