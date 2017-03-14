#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QProcess>
#include <QDebug>
#include <QTime>
#include "circularbuffer.h"

class MyThread : public QThread{

    Q_OBJECT

signals:

    void workReady(void);

private slots:

    void kill(void){
        run_ = false;
    }

public:

    MyThread(CircularBuffer *buffer, QObject *parent =0):
        QThread(parent),
        sharedBuffer_(buffer),
        run_(true)
    {}


    void run(void){
        while(run_){
            QProcess process;
            QString line;
            process.start("ifconfig");
            process.waitForReadyRead();
            process.waitForFinished();

            do{
                line = process.readLine();
                sharedBuffer_->insert(line);
            } while(!line.isNull());

            emit workReady();
            delay(15);
        }
    }

    void delay(int n_sec){
        QTime waitTime = QTime::currentTime().addSecs(n_sec);
        while(QTime::currentTime() < waitTime){}
    }

private:
    CircularBuffer* sharedBuffer_;
    bool run_;

};

#endif // MYTHREAD_H
