#ifndef DATABASE_H
#define DATABASE_H

#include <Event.h>
#include <QObject>
#include <QProcess>
#include <QString>
#include <qlocalsocket.h>



class Database
{
public:
    Database(QObject *parent);
    Database(QString pathToDatabase, QObject *parent);
    QList<Event *>* Read();
    int Delete(int id);
    QList<Event*>* Update(Event *event);
    int Create(Event *newEvent);
    ~Database();
private:
    QLocalSocket *pipe;
    QString path;
    QObject *parent;
    QProcess *server = NULL;
    QList<Event *>* DeserializeData(std::string json);
};

#endif // DATABASE_H
