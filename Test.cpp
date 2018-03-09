#include "Test.h"

Test::Test()
{
    m_Proc11111 =  new AtcLib::Activity<Test>(this, &Test::CloudSendProc);
    m_Proc11111->start();

    m_Proc22222 =  new AtcLib::Activity<Test>(this, &Test::CloudRecvProc);
    m_Proc22222->start();
}

void Test::CloudSendProc()
{
    DisplayThreadInfo("m_Proc11111");

    QString CurTimeStr;
    QDateTime CurTime, PreTime = QDateTime::currentDateTime();
    int icount = 0;
    while(!m_Proc11111->isStopped())
    {
        CurTime = QDateTime::currentDateTime();
        CurTimeStr = QString("m_Proc11111 contine----") + CurTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
        qDebug()<<CurTimeStr;
        icount++;
        //if(CurTime.toTime_t() - PreTime.toTime_t() == 10)
        if(icount%10 == 0)
        {
            qDebug()<<"m_Proc2222 msleep(5000)";
//            m_Proc22222->quit();
//            m_Proc22222->wait();
//            m_Proc22222->deleteLater();
            //m_Proc22222->start();
            m_Proc22222->msleep(50);
            continue;
        }
        m_Proc11111->msleep(10);
    }
    qDebug()<<"m_Proc11111 end";
}

void Test::CloudRecvProc()
{
    DisplayThreadInfo("m_Proc22222");

    QString CurTimeStr;
    QDateTime CurTime, PreTime = QDateTime::currentDateTime();
    while(!m_Proc22222->isStopped())
    {
        CurTime = QDateTime::currentDateTime();
        CurTimeStr = QString("m_Proc22222 ") + CurTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
        qDebug()<<CurTimeStr;
        m_Proc22222->msleep(10);
    }
    qDebug()<<"m_Proc22222 end";
}

