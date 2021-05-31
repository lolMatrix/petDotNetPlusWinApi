#include "database.h"

#include <QDebug>
#include <QJsonArray>
#include <QList>
#include <QProcess>
#include <qjsondocument.h>



Database::Database(QString pathToDatabase, QObject *parent)
{
    path = pathToDatabase;
    this->parent = parent;
}
//чтение из бд
QList<Event *>* Database::Read()
{
    QProcess *server =  new QProcess(parent);
    server->start(path, QStringList("-r"));
    server->waitForFinished();
    QList<Event *> *events;
    if(server->canReadLine())
    {
        QByteArray buff = server->readLine();
        std::string json = buff.toStdString();
        events = DeserializeData(json);
    }
    return events;
}
//удаление из бд
int Database::Delete(int id)
{
    QProcess *server =  new QProcess(parent);
    QList<QString> args;
    args.append(QString("-d"));
    args.append(QString::number(id));
    server->start(path, QStringList(args));
    server->waitForFinished();
    return server->exitCode();
}
//редактирование записи в бд
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
//создание записи в бд
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
//десериализация из ДЖОСОНА
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
