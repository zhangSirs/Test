#ifndef COMM_H
#define COMM_H

#include <QByteArray>
#include <vector>
#include <QString>
#include <QDateTime>
#include <QDebug>

#ifdef Q_OS_WIN

#include <WinSock2.h>
#include <windows.h>

#else

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <unistd.h>

#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#endif

typedef signed char            Int8;
typedef unsigned char          UInt8;
typedef signed short           Int16;
typedef unsigned short         UInt16;
typedef signed int             Int32;
typedef unsigned int           UInt32;
typedef signed long            IntPtr;
typedef unsigned long          UIntPtr;

#if defined(__LP64__)
    #define POCO_PTR_IS_64_BIT 1
    #define POCO_LONG_IS_64_BIT 1
    typedef signed long        Int64;
    typedef unsigned long      UInt64;
#else
    typedef signed long long   Int64;
    typedef unsigned long long UInt64;
#endif


#define MAX_ARRAY  128  //定义升级服务参数结构体中用户名等参数最大长度

//#pragma pack(1)
//IPC帧结构
typedef struct
{
    UInt8 Head;//帧头
    UInt16 FrameIndex;//帧序号
    UInt16 CmdCode;//命令码
    UInt8 ErrFlag;//异常标志
    UInt16 Len;//数据长度
    UInt8 Data[1024];//数据域
    UInt8 ChkSum;//校验和
    UInt8 Tail;//帧尾
}__attribute__((packed, aligned(1))) IPCFrameStruct;

//软件版本结构体信息
typedef struct
{
    UInt8 Major_Number;  //主版本号 1字节
    UInt8 Minor_Number;  //子版本号 1字节
    UInt8 Revision_Number;//修正版本号 1字节
    UInt8 Build_Number;	 //编译版本号 1字节
}__attribute__((packed, aligned(1))) SoftVersion;

//升级参数结构体
typedef struct
{
    UInt32 ip;      //升级服务器
    UInt16 port;    //升级端口号
    UInt8 lenUsr;   //登陆升级服务器用户名长度
    UInt8 lenPwd;   //登陆升级服务器密码长度
    UInt8 lenPath;  //升级文件路径长度
    char usr[MAX_ARRAY];        //用户名
    char password[MAX_ARRAY];   //密码
    char path[MAX_ARRAY];       //路径
    char sationCode[8];  //站编码
    char MD5[32];//升级压缩包校验码
}__attribute__((packed, aligned(1))) UpgradeStructData;
//#pragma pack()

#include <QEvent>

#define USER_MSG 1001

extern SoftVersion Version;

extern void VersionInit(SoftVersion &Version, UInt8 Major, UInt8 Minor, UInt8 Revision, UInt8 Build);
extern void DelayMs(float ms);//延时(ms)

extern UInt8 CalcSum_Byte(UInt8 * buffer, UInt16 length, bool GenOrCheckFlag);//校验和计算—— 一个字节
extern int PackIPCData(UInt8 *buf, IPCFrameStruct &IPCMsg);//打包IPC消息
extern bool UnPackIPCData(IPCFrameStruct &IPCMsg, UInt8 *buf, int DataLen);//解包IPC消息
extern QString DisplayThreadInfo(QString Name, int EigenValue=0);//打印线程信息

#endif//COMM_H
