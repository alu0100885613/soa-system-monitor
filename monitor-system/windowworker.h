#ifndef WINDOWWORKER_H
#define WINDOWWORKER_H
#include <QObject>
#include <QProcess>
#include <QDebug>

class WindowWorker: public QObject {

    Q_OBJECT

signals:
    void workFinished(QByteArray data);

public slots:
    void doWork(void){

        QProcess process;
        process.start("lshw -json");

        process.waitForReadyRead();

        QByteArray data = process.readAll();
        process.waitForFinished();

        emit workFinished(data);      
    }
};

#endif
