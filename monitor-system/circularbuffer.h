#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include <QString>
#include <QWaitCondition>
#include <QMutex>
#include <QVector>

class QVector<class T>;

class CircularBuffer{

public:

    CircularBuffer(int sz):
        size_(sz),
        pEx_(0),
        pIn_(0),
        buffer(size_)
    {
    }

    ~CircularBuffer(void){
    }

    QString extract(void){
        mutex_.lock();

        if(empty())
           bufferNotEmpty_.wait(&mutex_);

        QString toReturn = buffer[pEx_%size_];
        pEx_++;

        mutex_.unlock();

        return toReturn;
    }

    void insert(QString s){
        mutex_.lock();

        buffer[pIn_%size_] = s;
        pIn_++;
        bufferNotEmpty_.wakeAll();

        mutex_.unlock();
    }

    bool empty(){
        if(pIn_ == pEx_)
            return true;
        else
            return false;
    }

private:

    int size_;
    int pEx_;
    int pIn_;
    QVector<QString> buffer;
    QWaitCondition bufferNotEmpty_;
    QMutex mutex_;
};


#endif // CIRCULARBUFFER_H
