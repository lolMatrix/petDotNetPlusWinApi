#ifndef UPDATEARTICLETHREAD_H
#define UPDATEARTICLETHREAD_H

#include <MainWindow.h>
#include <QThread>



class UpdateArticleThread : public QThread
{
private:
    MainWindow *p;
public:
    UpdateArticleThread(MainWindow *parent);

    // QThread interface
protected:
    void run();
};

#endif // UPDATEARTICLETHREAD_H
