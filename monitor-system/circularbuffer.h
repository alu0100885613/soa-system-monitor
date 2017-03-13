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
        begin_(0),
        end_(size_-1),
        buffer(size_)
    {
    }

    ~CircularBuffer(void){
    }

    QString extract(void){
        mutex_.lock();

        if(empty())
           bufferNotEmpty_.wait(&mutex_);

        begin_--;
        QString toReturn = buffer[begin_%size_];

        mutex_.unlock();

        return toReturn;
    }

    void insert(QString s){
        mutex_.lock();

        buffer[begin_%size_] = s;
        begin_++;
        bufferNotEmpty_.wakeAll();

        mutex_.unlock();
    }

    bool empty(){
        if(begin_ == 0)
            return true;
        else
            return false;
    }

private:

    int size_;
    int begin_;
    int end_;
    QVector<QString> buffer;
    QWaitCondition bufferNotEmpty_;
    QMutex mutex_;
};


#endif // CIRCULARBUFFER_H
