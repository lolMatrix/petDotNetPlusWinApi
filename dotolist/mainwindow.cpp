#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMessageBox>
#include <QString>
#include <QString>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <qlist.h>
#include <QDebug>
#include <QProcess>
#include <QStringList>
#include <Database.h>
#include <UpdateArticleThread.h>
#include "regexvalidator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    events = new QList<Event *>();

    ui->dateTimeEvent->setMaximumDateTime(QDateTime::currentDateTime().addYears(1));
    ui->dateTimeEvent->setMinimumDateTime(QDateTime::currentDateTime());

    RegexValidator *val = new RegexValidator();
    ui->location->setValidator(val);

    ui->period->setDisabled(!ui->hasPeriod->isChecked());
    ui->repeatCount->setDisabled(!ui->hasPeriod->isChecked());

    ui->articleBrowser->setSelectionBehavior(QTableView::SelectRows);
    ui->articleBrowser->horizontalHeader()->setHighlightSections(false);


    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->hasPeriod, SIGNAL(clicked()), this, SLOT(setActivePeriods()));
    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(ui->add, SIGNAL(clicked()), this, SLOT(addArticle()));
    connect(ui->deleteCurrent, SIGNAL(clicked()), this, SLOT(deleteArticle()));
    connect(ui->articleBrowser, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(getArticle()));
    connect(ui->tenArticles, SIGNAL(clicked()), this, SLOT(setTenArticles()));
    connect(ui->menu->actions()[0], &QAction::triggered, this, &MainWindow::startServer);
    connect(ui->menu->actions()[1], &QAction::triggered, this, &MainWindow::stopServer);
    connectDatabase();
}

MainWindow::~MainWindow()
{
    stopServer();
    delete ui;
}

void MainWindow::readFromDataBase()
{
    QList<Event*> *events = db->Read();
    if(events->count() > 0)
    {
        ui->editor->setEnabled(true);
        ui->deleteCurrent->setEnabled(true);
        for (int i = 0; i < events->count(); i++)
        {
            SetData(events->value(i), i);
        }
    }
}

void MainWindow::clearBrowser()
{
    for (int i = events->count() - 1; i >= 0; i--) {
        ui->articleBrowser->removeRow(i);
    }

    ui->editor->setEnabled(false);
    ui->deleteCurrent->setEnabled(false);

    for (int i = events->count() - 1; i >= 0; i--){
        Event *e = events->takeAt(i);
        delete e;
    }
}

void MainWindow::connectDatabase()
{
    if(db == NULL){
        db = new Database(this);
        updateThread = new UpdateArticleThread(this);
        updateThread->start();
    }
}

void MainWindow::loadData(Event *event)
{
    if (checkData(ui->hasPeriod->isChecked()))
    {
        loadIntoEvents.setDurationEvent(event, ui->timeEvent->time());
        loadIntoEvents.setDateAndTimeEvent(event, ui->dateTimeEvent->dateTime());
        loadIntoEvents.setPeriodFlag(event, ui->hasPeriod->isChecked());
        if(ui->hasPeriod->isChecked())
        {
            loadIntoEvents.setRepeatCount(event, ui->repeatCount->value());
        }
        loadIntoEvents.setEventName(event, ui->name->text());
        loadIntoEvents.setLocation(event, ui->location->text());
        db->Update(event);
        getData(event);
        updateData();
    }
}

void MainWindow::getData(Event *event)
{
    ui->name->setText(event->name);
    ui->location->setText(event->location);
    ui->timeEvent->setTime(event->duration);
    ui->dateTimeEvent->setDateTime(event->getEventDate());
    ui->hasPeriod->setChecked(event->period.hasRepeat);
    ui->period->setEnabled(ui->hasPeriod->isChecked());
    ui->repeatCount->setEnabled(ui->hasPeriod->isChecked());
    getPeriod(event);
    ui->repeatCount->setValue(event->period.repeatCount);
    getImportance(event);

}



void MainWindow::load()
{
    loadData(events->value(ui->articleBrowser->currentRow()));
}

void MainWindow::setActivePeriods()
{
    ui->period->setEnabled(ui->hasPeriod->isChecked());
    ui->repeatCount->setEnabled(ui->hasPeriod->isChecked());
}

void MainWindow::cancel()
{
    getData(events->value(ui->articleBrowser->currentRow()));
}

void MainWindow::setTenArticles()
{
    for (int i = 0; i < 10; i++){
        addArticle();
    }
}

void MainWindow::getPeriod(Event *event)
{
    switch (event->period.periodEvent)
    {
    case event->period.everyDay:
        ui->everyDay->setChecked(true);
        break;
    case event->period.acrossDay:
        ui->acrossDay->setChecked(true);
        break;
    case event->period.acrossTwoDays:
        ui->acrossTwoDays->setChecked(true);
        break;
    case event->period.acrossTreeDays:
        ui->acrossTreeDays->setChecked(true);
        break;
    case event->period.everyWeak:
        ui->everyWeak->setChecked(true);
        break;
    case event->period.everyMouth:
        ui->everyMouth->setChecked(true);
        break;
    case event->period.everyQuarter:
        ui->everyQuarter->setChecked(true);
        break;
    case event->period.everyHalfYear:
        ui->everyHalfYear->setChecked(true);
        break;
    case event->period.everyYear:
        ui->everyYear->setChecked(true);
        break;
    }
}

void MainWindow::getImportance(Event *event){
    switch (event->eventImportance)
    {
    case Event::noImportant:
        ui->noImportant->setChecked(true);
        break;
    case Event::important:
        ui->important->setChecked(true);
        break;
    case Event::veryImportant:
        ui->veryImportant->setChecked(true);
        break;
    }
}

bool MainWindow::checkData(bool periodFlag)
{
    bool result = true;
    QObjectList importanceList = ui->importance->children();
    QObjectList periodList = ui->period->children();
    if(ui->name->text().length() <= 0)
    {
        QMessageBox::warning(this, "Не введено значение", "Введите имя мероприятия.");
        result = false;
    }
    else if (ui->location->text().length() <= 0)
    {
         QMessageBox::warning(this, "Не введено значение", "Введите место проведения мероприятия.");
         result = false;
    }
    else if (!loadIntoEvents.setImportance(events->value(ui->articleBrowser->currentRow()), importanceList))
    {
         QMessageBox::warning(this, "Не введено значение", "Выберите важность мероприятия");
         result = false;
    }else if (ui->timeEvent->time() == QTime(0, 0))
    {
        QMessageBox::warning(this, "Не введено значение", "Протяженность не может быть равна нулю");
        result = false;
    }
    else if (periodFlag){
        if(!loadIntoEvents.setPeriod(events->value(ui->articleBrowser->currentRow()), periodList))
        {
             QMessageBox::warning(this, "Не введено значение", "Выберите важность мероприятия");
             result = false;
        }
    }
    return result;
}

void MainWindow::deleteArticle()
{
    int rowIndex = ui->articleBrowser->currentRow();
    ui->articleBrowser->removeRow(rowIndex);
    int code = db->Delete(events->value(rowIndex)->id);
    events->removeOne(events->value(rowIndex));
    shorting();
    if (events->count() == 0)
    {
        ui->editor->setEnabled(false);
        ui->deleteCurrent->setEnabled(false);
    }

    if (rowIndex < events->count())
    {
        ui->articleBrowser->setCurrentCell(rowIndex, 0);
    }
    else
    {
        ui->articleBrowser->setCurrentCell(events->count() - 1, 0);
    }

    ui->articleBrowser->setFocus();
}

void MainWindow::addArticle()
{
    if(events->count() <= 0){
        ui->editor->setEnabled(true);
        ui->deleteCurrent->setEnabled(true);
    }
    Event *newEvent = new Event();
    newEvent->name = "New Event" + QString::number(events->count());
    int row = events->count();
    SetData(newEvent, row);
    db->Create(newEvent);
}

void MainWindow::getArticle()
{
    int position = ui->articleBrowser->currentRow();
    if (position >= 0){
        getData(events->value(position));
    }
}

void MainWindow::setFlags(QTableWidgetItem *cell){
    Qt::ItemFlags flags = cell->flags();
    flags.setFlag(Qt::ItemIsEditable, false);
    cell->setFlags(flags);
}

void MainWindow::updateData()
{
    int row = ui->articleBrowser->currentRow();
    Event *e = events->value(row);
    QTableWidgetItem *name = ui->articleBrowser->item(row, 0);
    QTableWidgetItem *dateTime = ui->articleBrowser->item(row, 1);
    QTableWidgetItem *importance = ui->articleBrowser->item(row, 2);
    name->setText(e->name);
    dateTime->setText(e->getEventDate().toString());
    importance->setText(e->getImportanceString(e->eventImportance));
    shorting();
    updateAllTable();
    ui->articleBrowser->setCurrentCell(events->indexOf(e), 0);
    ui->editor->setFocus();
}

void MainWindow::shorting()
{
    for (int i = 0; i < events->count(); i++){
        for (int j = 0; j < events->count() - i-1; j++){
            if (*events->value(j) > *events->value(j+1)){
                    Event *buff = events->value(j);
                    events->replace(j, events->value(j+1));
                    events->replace(j+1, buff);
            }
        }
    }

    updateAllTable();
}

void MainWindow::updateAllTable()
{
    for(int i = 0; i < events->count(); i++){
        QTableWidgetItem *name = ui->articleBrowser->item(i, 0);
        QTableWidgetItem *date = ui->articleBrowser->item(i, 1);
        QTableWidgetItem *importance = ui->articleBrowser->item(i, 2);
        name->setText(events->value(i)->name);
        date->setText(events->value(i)->getEventDate().toString());
        importance->setText(events->value(i)->getImportanceString(events->value(i)->eventImportance));
    }
}

void MainWindow::SetData(Event *newEvent, int row)
{
    events->append(newEvent);
    ui->articleBrowser->setRowCount(events->count());
    QTableWidgetItem *name = new QTableWidgetItem(newEvent->name);
    setFlags(name);
    QTableWidgetItem *dateEvent = new QTableWidgetItem(newEvent->getEventDate().toString());
    setFlags(dateEvent);
    QTableWidgetItem *important = new QTableWidgetItem(newEvent->getImportanceString(newEvent->eventImportance));
    setFlags(important);
    ui->articleBrowser->setItem(row, 0, name);
    ui->articleBrowser->setItem(row, 1, dateEvent);
    ui->articleBrowser->setItem(row, 2, important);
    shorting();
    ui->articleBrowser->setCurrentCell(events->indexOf(newEvent), 0);
    ui->articleBrowser->setFocus();
}

void MainWindow::startServer()
{
    updateThread->quit();
    updateThread->wait();
    delete updateThread;
    clearBrowser();
    delete db;
    db = NULL;

    if(db == NULL){
        char fileName[] = "D:\\Users\\drles\\source\\repos\\todolistserver\\todolistserver\\bin\\Debug\\netcoreapp3.1\\todolistserver.exe";
        db = new Database(fileName, this);
        readFromDataBase();
        updateThread = new UpdateArticleThread(this);
        updateThread->start();
    }
}



void MainWindow::stopServer()
{
    clearBrowser();
    if (db != NULL){
        updateThread->quit();
        updateThread->wait();
        delete updateThread;
        delete db;
        db = NULL;
    }
}

