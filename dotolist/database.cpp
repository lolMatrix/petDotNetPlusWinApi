#include "database.h"

#include <QDebug>
#include <QJsonArray>
#include <QList>
#include <QLocalSocket>
#include <QProcess>
#include <qjsondocument.h>
#include <QThread>


Database::Database(QString pathToDatabase, QObject *parent)
{
    path = pathToDatabase;
    this->parent = parent;
    server = new QProcess(parent);
    server->start(path);
    while( server->state() != QProcess::Running)
    {
        QThread::msleep(100);
    }
    int a = 0;
}

QList<Event *>* Database::Read()
{

    QLocalSocket socket(parent);
    socket.connectToServer("todolistserverepta");
    socket.waitForConnected();
    QList<Event *> *events;
    socket.write("-r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    QByteArray buff = socket.readLine();
    std::string json = buff.toStdString();
    events = DeserializeData(json);
    return events;
}

int Database::Delete(int id)
{
    QList<QString> args;
    args.append(QString("-d"));
    args.append(QString::number(id));
    server->waitForFinished();
    return server->exitCode();
}

int Database::Update(Event *event)
{
    QProcess *server =  new QProcess(parent);

    QList<QString> args;
    args.append(QString("-u"));
    args.append(QString::number(event->id));

    QJsonDocument serializedData;
    serializedData.setObject(event->Serialize());
    args.append(QString::fromUtf8(serializedData.toJson(QJsonDocument::Compact)));

    server->start(path, QStringList(args));
    server->waitForFinished();

    return server->exitCode();
}

int Database::Create(Event *newEvent)
{
    QProcess *server =  new QProcess(parent);
    QList<QString> args;
    args.append(QString("-c"));
    QJsonDocument serializedData;
    serializedData.setObject(newEvent->Serialize());
    args.append(QString::fromUtf8(serializedData.toJson()));
    server->start(path, QStringList(args));
    server->waitForFinished();
    char id[100];
    server->readLine(id, 100);
    newEvent->id = QString(id).toInt();
    return server->exitCode();
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
