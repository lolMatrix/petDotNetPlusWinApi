#ifndef DATABASE_H
#define DATABASE_H

#include <Event.h>
#include <QObject>
#include <QProcess>
#include <QString>



class Database
{
public:
    Database(QString pathToDatabase, QObject *parent);
    QList<Event *>* Read();
    int Delete(int id);
    int Update(Event *event);
    int Create(Event *newEvent);
private:
    QString path;
    QObject *parent;
    QProcess *server;
    QList<Event *>* DeserializeData(std::string json);
};

#endif // DATABASE_H
