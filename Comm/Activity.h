#ifndef ACTIVITY_H
#define ACTIVITY_H


#include <QThread>
//#include <QMutex>
#include <QMutexLocker>


#ifdef Q_OS_LINUX
    #include <time.h>
    #include <unistd.h>
    #include "pthread.h"
#endif


namespace AtcLib
{

template<class C>
class Activity:public QThread
{
public:
    typedef void (C::*CallBack)();
    Activity(C* object,CallBack method):_pObject(object),_method(method),
        _stopped(true),
        _running(false)
    {
    }

    virtual ~Activity()
    {
        stop();
        wait();
    }

    void start()
    {
         QMutexLocker locker(&_mutex);
         if(!_running)
         {
             try
             {
                 QThread::start();
                 _stopped = false;
                 _running = true;
             }
             catch(...)
             {
                 _running = false;
                 throw;
             }

         }
    }

    void stop()
    {
        _stopped = true;
        _running = false;

        QMutexLocker locker(&_mutex);
    }

    void wait(long milliseconds)
    {
        if(_running)
        {
            QMutexLocker locker(&_mutex);
        }

    }

    void wait()
    {
        QMutexLocker locker(&_mutex);
    }

    bool isStopped() const
    {
        return _stopped;
    }

    bool isRunning() const
    {
        return _running;
    }

    void msleep(/*unsigned*/ long int microseconds)
    {

        #ifdef Q_OS_WIN
            QThread::msleep(microseconds);
        #else
            struct timespec ts = { (microseconds/1000), (microseconds % 1000) * 1000 * 1000 };
            while(nanosleep(&ts, NULL) == -1){};
        #endif
    }

    void sleep(unsigned long sec)
    {

    #ifdef Q_OS_WIN
        QThread::sleep(sec);
    #else
        struct timespec ts = { sec, 0 };
        while(nanosleep(&ts, NULL) == -1){};
    #endif

    }

protected:
    void run()
    {
        QMutexLocker locker(&_mutex);
        try
        {

            (_pObject->*_method)();

        }
        catch(...)
        {
            _running = false;
            throw;

        }

        _running = false;
    }

private:
    C* _pObject;
    CallBack _method;

    volatile bool _stopped;
    volatile bool _running;

    QMutex _mutex;
};

}

#endif // ACTIVITY_H
