#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include <QString>
#include <QWaitCondition>
#include <QMutex>

class CircularBuffer{

public:
    CircularBuffer(void):
        follower_(-1),
        size_(20),
        buffer(new QStringList)
    {}

    ~CircularBuffer(void){
        if(buffer != NULL)
            delete buffer;
        buffer = NULL;
    }

    QString extract(void){
        mutex_.lock();

        if(follower_ != -1)
            bufferNotEmpty_.wait(&mutex);

        follower_--;
        QString toReturn = buffer[follower_%size_];

        mutex_.unlock();

        return toReturn;
    }

    void insert(QString s){
        mutex_.lock();

        follower_++;
        buffer[follower_%size_] = s;
        bufferNotEmpty_.wakeAll();

        mutex_.unlock();
    }

private:
    int follower_;
    int size_;
    QStringList* buffer;

    QWaitCondition bufferNotEmpty_;
    QMutex mutex_;
};


#endif // CIRCULARBUFFER_H
