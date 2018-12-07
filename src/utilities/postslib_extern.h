#ifndef postslib_extern_h_
#define postslib_extern_h_
#include <stddef.h>
#include <stdint.h>
enum {
	IMAGE_BMP = 0,   // bmp单色文件格式
	IMAGE_PNG,       // png 文件格式
	IMAGE_JBG,
};

#define     PORT_RET_OK             0x00        //  成功
#define     PORT_RET_NOTEMPTY       0x01        //  发送缓冲还有数据
#define     PORT_RET_PORTERR        0x02        //  逻辑串口号非法
#define     PORT_RET_NOTOPEN        0x03        //  串口未打开
#define     PORT_RET_NOTOPEN        0x03        //  串口未打开
#define     PORT_RET_BUSY           0x05        //  没有物理串口资源分配
#define     PORT_RET_PARAERR        0xfe        //  无效的通讯参数
#define     PORT_COM1				0x00        //  下载口
#define     OK						0x00
typedef     unsigned char           UINT8;
typedef     int                     INT32;
typedef     unsigned int            UINT32;

#define	KEY_CANCEL              DIKS_ESCAPE		//  取消

#define  MAX_PACKET_LEN 2048*5+10

#define  E_MEM_ERR        1   //文件操作失败

#ifndef  STX
#define  STX             0x02
#define  ETX             0x03
#endif

 int portOpen(int PortNum, const char *PortPara);
 int portClose(int PortNum);
 int portSends(int PortNum, const void *SendBuf, size_t SendLen);
 int portRecv(int PortNum, uint8_t *RecvBuf, uint32_t TimeOutMs);
 int portCheckRecvBuf(int PortNum);
 int SetElecSignTimeOut(uint16_t usTimeOutMs);
 int PedSignature(char *TxnCode, char *ImagefileName, uint8_t ImageType, uint32_t timeOutMs,char *szPrompt);
uint32_t sysGetTimerCount(void);

#endif
