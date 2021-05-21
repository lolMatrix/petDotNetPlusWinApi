#include "updatearticlethread.h"

UpdateArticleThread::UpdateArticleThread(MainWindow *parent)
{
    p = parent;
}

void UpdateArticleThread::run()
{
    while(1){
        p->readFromDataBase();
        wait(1000);
    }
}
