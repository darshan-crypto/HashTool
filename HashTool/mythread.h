#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include<mainwindow.h>
#include<QComboBox>

class mythread : public QThread
{

public:
    mythread(MainWindow);
    void run() override;
    std::string s;
    QComboBox *c;
signals:
    void hashready(QString);
};

#endif // MYTHREAD_H
