#include "database.h"

#include <QDebug>
#include <QJsonArray>
#include <QList>
#include <QLocalSocket>
#include <QProcess>
#include <qjsondocument.h>
#include <QThread>


Database::Database(QObject *parent)
{
    pipe = new QLocalSocket(parent);

    pipe->connectToServer("todolistserverepta");
    pipe->waitForConnected();
}

Database::Database(QString pathToDatabase, QObject *parent)
{
    path = pathToDatabase;
    this->parent = parent;
    server = new QProcess(parent);
    server->start(path);
    QThread::msleep(1500);

    pipe = new QLocalSocket(parent);

    pipe->connectToServer("todolistserverepta");
    pipe->waitForConnected();
}

QList<Event *>* Database::Read()
{
    QList<Event *> *events;
    pipe->write("-r\n");
    pipe->waitForBytesWritten();
    pipe->waitForReadyRead();
    QByteArray buff = pipe->readLine();
    std::string json = buff.toStdString();
    events = DeserializeData(json);
    return events;
}

int Database::Delete(int id)
{
    pipe->write("-d\n");
    pipe->write(QString::number(id).append('\n').toStdString().c_str());
    pipe->waitForBytesWritten();
    return 0;
}

QList<Event*>* Database::Update(Event *event)
{
    pipe->write("-u\n");
    pipe->write(QString::number(event->id).append('\n').toStdString().c_str());

    QJsonDocument serializedData;
    serializedData.setObject(event->Serialize());
    pipe->write(QString::fromUtf8(serializedData.toJson(QJsonDocument::Compact)).append('\n').toStdString().c_str());

    pipe->waitForBytesWritten();

    return Read();
}

int Database::Create(Event *newEvent)
{
    pipe->write("-c\n");
    QJsonDocument serializedData;
    serializedData.setObject(newEvent->Serialize());
    pipe->write(QString::fromUtf8(serializedData.toJson(QJsonDocument::Compact)).append('\n').toStdString().c_str());
    pipe->flush();
    pipe->waitForBytesWritten();
    pipe->waitForReadyRead();
    QByteArray buff = pipe->readLine();
    QString id = QString::fromUtf8(buff);
    newEvent->id = id.toInt();
    return 0;
}

Database::~Database()
{
    if(server != NULL){
        if(server->isOpen())
            server->close();

        delete server;
    }

    pipe->close();

    delete pipe;
}

QList<Event *>* Database::DeserializeData(std::string json)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(QString::fromStdString(json).toUtf8());
    QJsonArray objects = jsonResponse.array();
    QList<Event *> *events = new QList<Event *>();
    for (int i = 0; i < objects.count(); i++){
        Event *newEvent = Event::Deserialize(objects[i].toObject(), i);
        events->append(newEvent);
    }
    return events;
}
