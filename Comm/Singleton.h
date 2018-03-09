#ifndef SINGLETON_H
#define SINGLETON_H

#include <QObject>

template <class T>
class Singleton
{
public:
    static T& instance()
    {
        return *m_instance;
    }

private:
    static T* m_instance;
    Singleton(){}
};

//外部初始化 before invoke main
template <class T>
T* Singleton<T>::m_instance = new T;

#endif // SINGLETON_H
