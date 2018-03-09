#include "Comm.h"

SoftVersion Version;

//延时(ms)
void DelayMs(float ms)
{
#ifdef Q_OS_LINUX
    struct timeval tv;
    long long total_usec;

    total_usec = ms * 1000;
    tv.tv_sec = total_usec/1000000;
    tv.tv_usec = total_usec - tv.tv_sec*1000000;

    select(0, NULL, NULL, NULL, &tv);
#endif

#ifdef Q_OS_WIN
    Sleep(ms);
#endif
}

//校验和计算—— 一个字节
UInt8 CalcSum_Byte(UInt8 * buffer, UInt16 length, bool GenOrCheckFlag)
{
   UInt8 sum;
   UInt16 i;

   sum = 0;
   for(i=0 ;i<length ;i++)
   {
       sum += buffer[i];
   }

   if(GenOrCheckFlag)//如果是生成校验码
   {
       buffer[length] = sum;
       return sum;
   }
   else//如果是检查校验码
   {
       sum -= buffer[length-1];//减去帧尾
       sum -= buffer[length-2];//减去校验和
       if(sum == buffer[length-2])//与收到的校验和比较
       {
           return TRUE;
       }
       else
       {
           return FALSE;
       }
   }
}

//打包IPC消息
int PackIPCData(UInt8 *buf, IPCFrameStruct &IPCMsg)
{
    UInt16 Len;
    UInt8 ChkSum;

    Len = 0;
    buf[Len++] = IPCMsg.Head;
    buf[Len++] = IPCMsg.FrameIndex&0xFF;
    buf[Len++] = IPCMsg.FrameIndex>>8;
    buf[Len++] = IPCMsg.CmdCode&0xFF;
    buf[Len++] = IPCMsg.CmdCode>>8;
    buf[Len++] = IPCMsg.ErrFlag;
    buf[Len++] = IPCMsg.Len&0xFF;
    buf[Len++] = IPCMsg.Len>>8;
    memcpy(&buf[8], &IPCMsg.Data[0], IPCMsg.Len);
    Len += IPCMsg.Len;

    ChkSum = CalcSum_Byte(buf, Len, true);
    buf[Len++] = ChkSum;
    buf[Len++] = IPCMsg.Tail;

    return Len;
}

//解包IPC消息
bool UnPackIPCData(IPCFrameStruct &IPCMsg, UInt8 *buf, int DataLen)
{
    UInt16 Len;

    if(!CalcSum_Byte(buf, DataLen, false))//校验失败
    {
        return false;
    }

    Len = 0;
    IPCMsg.Head = buf[Len++];

    IPCMsg.FrameIndex = buf[Len]+(UInt16)(buf[Len+1]<<8);
    Len += 2;

    IPCMsg.CmdCode = buf[Len]+(UInt16)(buf[Len+1]<<8);
    Len += 2;

    IPCMsg.ErrFlag = buf[Len++];

    IPCMsg.Len = buf[Len]+(UInt16)(buf[Len+1]<<8);;
    Len += 2;

    memcpy(&IPCMsg.Data[0], &buf[8], IPCMsg.Len);
    Len += IPCMsg.Len;

    Len++;//CHKSUM Len = 1

    IPCMsg.Tail = buf[Len++];

    return true;
}


void VersionInit(SoftVersion& Version, UInt8 Major, UInt8 Minor, UInt8 Revision, UInt8 Build)
{
        Version.Major_Number = Major;
        Version.Minor_Number = Minor;
        Version.Revision_Number = Revision;
        Version.Build_Number = Build;
}

//打印线程信息
QString DisplayThreadInfo(QString Name, int EigenValue)
{
    if(EigenValue != 0)
        Name += QString("-%1").arg(EigenValue);

     QByteArray ByteArray = Name.toLatin1();
     const char *ThreadName = ByteArray.data();

    QString InfoStr;
#ifdef Q_OS_LINUX
    if(prctl(PR_SET_NAME, (unsigned long)ThreadName) == 0)
    {
        int threadId = gettid();

        InfoStr = QString("Thread: Id=%1, CreateTime=%2, Name=%3").arg(\
                    QString::number(threadId), \
                    QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"),\
                    Name);
    }
#endif

    qDebug()<<InfoStr;
    return InfoStr;
}
