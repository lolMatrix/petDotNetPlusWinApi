#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "event.h"

#include <Database.h>
#include <Loader.h>
#include <QMainWindow>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void readFromDataBase();
private:
    int currentArticle = -1;
    QTimer *timer = NULL;
    void clearBrowser();
    void getData(Event *event);
    Loader loadIntoEvents;
    Database *db = NULL;
    Ui::MainWindow *ui;
    QList<Event *> *events;
    int position;
    void loadData(Event *event);
    void getPeriod(Event *event);
    void getImportance(Event *event);
    bool checkData(bool periodFlag);
    void setFlags(QTableWidgetItem *cell);
    void updateData();
    void shorting();
    void updateAllTable();
    void DeserializeData(std::string json);
    void SetData(Event *newEvent, int row);
    void startNewTimer();
    void stopTimer();
private slots:
    void connectDatabase();
    void startServer();
    void stopServer();
    void deleteArticle();
    void addArticle();
    void getArticle();
    void load();
    void setActivePeriods();
    void cancel();
    void setTenArticles();
};
#endif // MAINWINDOW_H
