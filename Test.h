#ifndef TEST_H
#define TEST_H

#include "Comm/Activity.h"
#include "Comm/Comm.h"
#include <QDateTime>
#include <QDebug>

class Test
{
public:
    Test();

private:
    AtcLib::Activity<Test> * m_Proc11111;
    AtcLib::Activity<Test> * m_Proc22222;

    void CloudSendProc();//云平台发送函数
    void CloudRecvProc();//云平台发送函数
};

#endif // TEST_H
