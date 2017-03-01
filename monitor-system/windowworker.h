#ifndef WINDOWWORKER_H
#define WINDOWWORKER_H
#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QFile>

class WindowWorker: public QObject {

    Q_OBJECT

signals:
    void workFinished(void);

public slots:
    void doWork(void){

        QProcess process;
        process.start("lshw -json");

        process.waitForReadyRead();
        process.waitForFinished();
        byteArray_ = process.readAll();

        emit workFinished();
    }

    QByteArray get_byteArray(void){
        return byteArray_;
    }

private:
        QByteArray byteArray_;

};

#endif
