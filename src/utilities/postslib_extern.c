//#include "seos.h"
//#include "postslib.h"
#include "postslib_extern.h"

#include <stdint.h>
#include <string.h>
#include <posapi.h>
#include <time.h>
#include "uart.h"


#include "lcd.h"

static struct timeval	global_tick;			/* 用于程序执行时间计数 */
extern int portOpen(int PortNum, const char *PortPara);
extern int portClose(int PortNum);
extern int portSends(int PortNum, const void *SendBuf, size_t SendLen);
extern int portRecv(int PortNum, uint8_t *RecvBuf, uint32_t TimeOutMs);
extern int portCheckRecvBuf(int PortNum);
extern int SetElecSignTimeOut(uint16_t usTimeOutMs);
extern int PedSignature(char *TxnCode, char *ImagefileName, uint8_t ImageType, uint32_t timeOutMs,char *szPrompt);


/* 打开本应用所属的文件 */
int fileOpen(const char *filename, int oflag, ...)
{
	int fd;

	if (oflag & O_CREAT) {
		int mode = 0644;
#if 0
		va_list arg;

		va_start(arg, oflag);
		mode = va_arg(arg, int);
		va_end(arg);
#endif
		fd = open(filename, oflag, mode);
	} else {
		fd = open(filename, oflag);
	}

	return fd;
}/* 对打开的文件指针进行定位 */

int portCheckRecvBuf(int PortNum)
{
	return 1;
}/* 对打开的文件指针进行定位 */
int portSends(int PortNum, const void *SendBuf, size_t SendLen)
{
	return 1;
}/* 对打开的文件指针进行定位 */
int portRecv(int PortNum, uint8_t *RecvBuf, uint32_t TimeOutMs)
{
	return 1;
}/* 对打开的文件指针进行定位 */
int portClose(int PortNum)
{
	return 1;
}/* 对打开的文件指针进行定位 */
int portOpen(int PortNum, const char *PortPara)
{
	return 1;
}/* 对打开的文件指针进行定位 */

int fileSeek(int fd, off_t offset, int whence)
{
	return lseek(fd, offset, whence);
}

/* 关闭已打开的文件 */
int fileClose(int fd)
{
	return close(fd);
}

/* 从文件的当前定位位置开始写入数据 */
int fileWrite(int fd, const void *buf, size_t count)
{
	return safety_full_write(fd, buf, count);
}

/* 修改属于本应用的数据文件文件名 */
int fileRename(const char *OldFileName, const char *NewFileName)
{
	return rename(OldFileName, NewFileName);
}



uint32_t sysGetTimerCount(void)
{
	struct timeval	tn, ti;

	getruntime(&tn, NULL);
	timersub(&tn, &global_tick, &ti);

	return ti.tv_sec * 1000 + ti.tv_usec / 1000;
}

// 计算LRC
static uint8_t CalcLRC(uint8_t *psData, uint32_t uiLength, uint8_t ucInit)
{
	while( uiLength>0 )
	{
		ucInit ^= *psData++;
		uiLength--;
	}
	
	return ucInit;
}


static int RS232NacRxdForSign(uint8_t *psRxdData, uint32_t uiExpLen, uint32_t uiTimeOut, uint32_t *puiOutLen)
{
	INT32   iRet,iFlag,iCnt=0;
	UINT32	uiReadCnt, uiLength;
	UINT32  iOldTimeMs,iNewTimeMs,uiTemp;
	uint8_t	sWorkBuf[MAX_PACKET_LEN+50];

	if( uiExpLen>MAX_PACKET_LEN )
	{
		return -1;
	}

	iFlag = 0;
	uiTemp = 5000;
	uiReadCnt = uiLength = 0;
	memset(sWorkBuf, 0x00, sizeof(sWorkBuf));
	iOldTimeMs = sysGetTimerCount();
	while( 1 )
	{
		if (iFlag==0)
		{
			iNewTimeMs= sysGetTimerCount();			
			if( (iNewTimeMs-iOldTimeMs) >= (UINT32)(uiTimeOut) )	// 检查定时器
			{
				if( uiReadCnt>0 )	// 已经读取到数据
				{
					break;
				}
				return -1;
			}

			if( kb_hit())
			{
				if (kb_getkey() == KEY_CANCEL)
				{
					return KEY_CANCEL;
				}
			}

			iRet = portCheckRecvBuf(PORT_COM1);
			if (iRet != PORT_RET_NOTEMPTY)
			{
				if (iRet != PORT_RET_OK )
				{
					return iRet;
				}
				continue;
			}
		}

		iRet = portRecv(PORT_COM1, &sWorkBuf[uiReadCnt], uiTemp);//uiTemp
		if( iRet!=0 )
		{
			if( iRet==0xFF && iCnt <3)
			{
				iCnt++;
				continue;
			}
			return iRet;
		}
		uiTemp = 100;
		if (0 == iFlag)
		{
			if( sWorkBuf[0]!=STX )
			{
				continue;
			}

			iFlag = 1;
		}

		uiReadCnt++;
		if (uiReadCnt > uiExpLen)
		{
			return -1;
		}

		if( uiReadCnt==3 )
		{
			uiLength = (sWorkBuf[1]<<8)|sWorkBuf[2];

		}
		if( uiReadCnt==uiLength+4 ) //签字板协议有状态位
		{	// read data ok, verify it ...
			if( sWorkBuf[uiReadCnt-2]==ETX &&
				CalcLRC(&sWorkBuf[1], (UINT32)(uiReadCnt-1), 0)==0 )
			{
				break;
			}
			return -1;
		}
	}

	memcpy(psRxdData, &sWorkBuf[3], uiLength);
	if( puiOutLen!=NULL )
	{
		*puiOutLen = uiLength;
	}

	return 0;
}

// 串口直接发送
static int RS232RawTxd(uint8_t *psTxdData, uint32_t uiDataLen, uint32_t uiTimeOut)
{
	int 	iRet;
	int     iOldTime,iNowTime;
	
	if (uiTimeOut<=0)
	{
		uiTimeOut = 2;
	}
	iOldTime = sysGetTimerCount();
	iRet = 0;
	while (1)
	{
		iNowTime = sysGetTimerCount();
		if ((iNowTime-iOldTime) >= uiTimeOut*100)
		{
			iRet =  1;
			break;
		}
		
		iRet = portSends(PORT_COM1, psTxdData,uiDataLen);
		if( iRet == PORT_RET_OK )
		{
			break;
		}
	}
	
	return iRet;
}

static INT32 RS232NacTxd1(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	INT32		iRet;
	uint8_t		sWorkBuf[MAX_PACKET_LEN+50];

	if( uiDataLen>MAX_PACKET_LEN )
	{
		return -1;
	}

	sWorkBuf[0] = STX;

	sWorkBuf[1] = (uiDataLen>>8)&0xFF;
	sWorkBuf[2] = uiDataLen&0xFF;

	uiDataLen -=1;
	memcpy(&sWorkBuf[3], psTxdData, uiDataLen);
	sWorkBuf[3+uiDataLen]   = ETX;
	sWorkBuf[3+uiDataLen+1] = CalcLRC(psTxdData, uiDataLen, (uint8_t)(sWorkBuf[1] ^ sWorkBuf[2] ^ ETX));

	portClose(PORT_COM1);
	iRet = portOpen(PORT_COM1, (void *)"115200,8,N,1");
	if(iRet!=0)
	{
		return iRet;
	}
	iRet = RS232RawTxd(sWorkBuf, (UINT32)(uiDataLen+5), uiTimeOut);	// data
	if( iRet!=0 )
	{
		return iRet;
	}

	return 0;
}

int ShakeHands(void)
{
	int iRet, uiTimeOut;
	uint8_t	szSendBuf[MAX_PACKET_LEN], szRecvBuf[MAX_PACKET_LEN];
	uint32_t usSendLen=0, usRecvLen;
	
	memset(szSendBuf, 0, sizeof(szSendBuf));
	szSendBuf[0] = 0xA0;
	usSendLen = 1;
	usSendLen += 1; //长度包括ETX
	iRet = RS232NacTxd1(szSendBuf, usSendLen, 1);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}
	
	memset(szRecvBuf, 0x00, sizeof(szRecvBuf));
	uiTimeOut = 100;//超时100ms
	iRet = RS232NacRxdForSign(szRecvBuf, MAX_PACKET_LEN, uiTimeOut, &usRecvLen);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}
	
	if(memcmp(szRecvBuf, "\xB0\x01",2)==0)
	{
		return 0;
	}
	
	return 1;
}

static int SaveImageFile(char *filename, uint8_t *filebuf, uint32_t filelen)
{
	int fd;
	int ret;
	
	fd = fileOpen(filename, O_RDWR|O_CREAT);
	if( fd<0 ) 
	{
		return (E_MEM_ERR);
	}
	
	ret = fileSeek(fd, (int)0, SEEK_SET);
	if( ret<0 ) 
	{
		fileClose(fd);
		return (E_MEM_ERR);
	}
	ret = fileWrite(fd, (uint8_t *)filebuf, filelen);
	if( ret!=filelen )
	{
		fileClose(fd);
		return (E_MEM_ERR);
	}
	fileClose(fd);
	return (OK);
}

int InputSign(char *TxnCode, char *ImagefileName, uint8_t ImageType, uint32_t timeOutMs)
{
	int iRet;
	uint8_t	szSendBuf[MAX_PACKET_LEN], szRecvBuf[MAX_PACKET_LEN];
	uint32_t usSendLen=0, usRecvLen;
	uint8_t  BMP_SIGN[1024*10+20];
	UINT8 sBuf[1024*10+20];
	
	unsigned long ulBmpSignLen = 0;		   // 所获取到的bmp签名的长度

	memset(szSendBuf, 0, sizeof(szSendBuf));
	szSendBuf[0] = 0xA2;
	usSendLen = 1;

	sprintf((char*)&szSendBuf[1], "%.8s", TxnCode);
	usSendLen += 8;

	usSendLen += 1; //该终端需要把etx算上
	iRet = RS232NacTxd1(szSendBuf, usSendLen, 1);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}

	memset(szRecvBuf, 0, sizeof(szRecvBuf));
	iRet = RS232NacRxdForSign(szRecvBuf, MAX_PACKET_LEN, timeOutMs, &usRecvLen);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}

	if (szRecvBuf[0] == (uint8_t)'\xB2') //外接手写签名板是BMP格式
	{
		memset(BMP_SIGN, 0x00, sizeof(BMP_SIGN));
		memcpy(BMP_SIGN, &szRecvBuf[4], usRecvLen - 5);
		ulBmpSignLen = usRecvLen - 5;
		portClose(PORT_COM1);
		memcpy(sBuf,BMP_SIGN,ulBmpSignLen);
		//输出的bmp文件是黑底白字，转换成白底黑字，只需转换文件头的像素
		memcpy(&sBuf[54],"\x00\x00\x00\x00\xFF\xFF\xFF\x00",8);
		iRet=SaveImageFile(ImagefileName, sBuf, ulBmpSignLen);
		if(iRet!=OK)
			return iRet;
		//iRet=bmp2jbg("temp.bmp", ImagefileName);
		//fileRemove("temp.bmp");
		return iRet;
	}
	else if (memcmp(szRecvBuf, "\xC2\x03",2)==0)
	{
		portClose(PORT_COM1);
		return 1;
	}
	else
	{
		portClose(PORT_COM1);
		return 1;
	}

	return 0;
}

int AfterSignReply(UINT8 ucFlag)
{
	int iRet, uiTimeOut, iTry;
	uint8_t	szSendBuf[MAX_PACKET_LEN], szRecvBuf[MAX_PACKET_LEN];
	uint32_t usSendLen=0, usRecvLen;

	for (iTry = 0; iTry < 3; iTry++) // 最多尝试3次发起签名结束请求报文
	{
		memset(szSendBuf, 0, sizeof(szSendBuf));
		szSendBuf[0] = 0xA3;
		usSendLen = 1;
		usSendLen += 1; //该终端需要把etx算上
		iRet = RS232NacTxd1(szSendBuf, usSendLen, 1);
		if(iRet !=0)
		{
			continue;
		}

		if (1 == ucFlag) // 经过测试证明，如果是POS端主动取消签名，则pos端发出结束签名请求，而无需等待回应，手写板也不会有回应
		{
			return 0;
		}

		memset(szRecvBuf, 0, sizeof(szRecvBuf));
		uiTimeOut = 100;
		iRet = RS232NacRxdForSign(szRecvBuf, MAX_PACKET_LEN, uiTimeOut, &usRecvLen);
		if(iRet !=0)
		{
			continue;
		}

		if(memcmp(szRecvBuf, "\xB3", 1)==0)
		{
			return 0;
		}

	}
	portClose(PORT_COM1);
	return 1;
}

int PedSignature(char *TxnCode, char *ImagefileName, uint8_t ImageType, uint32_t timeOutMs,char *szPrompt)
{
    int iRet;
	
	//参数检查
	if(TxnCode==NULL || strlen(TxnCode)==0 
		|| ImagefileName==NULL || strlen(ImagefileName)==0
		|| (ImageType!=IMAGE_BMP && ImageType!=IMAGE_JBG))
	{
		return 1;
	}
	
	//默认40秒
	if(timeOutMs==0)
	{
		timeOutMs = 40000;
	}
	
	iRet = ShakeHands();
	if(iRet !=0)
	{
		return iRet;
	}
	
	
	lcd_clean();
	lcd_printf(ALG_CENTER, "%s", szPrompt);
	lcd_flip();

	iRet = InputSign(TxnCode, ImagefileName, ImageType, timeOutMs);
	if (iRet == KEY_CANCEL) //按了取消键
	{
		AfterSignReply(1);
		return iRet;
	}
	else
	{
		AfterSignReply(0); // 不管是否成功获取签名，也得发起结束签名命令
	}
	if(iRet !=0)
	{
		return iRet;
	}
	
	return 0;
}

int SetElecSignTimeOut(uint16_t usTimeOutMs)
{
	int iRet, uiTimeOut;
	uint8_t	szSendBuf[MAX_PACKET_LEN], szRecvBuf[MAX_PACKET_LEN];
	uint32_t usSendLen=0, usRecvLen;
	
	memset(szSendBuf, 0, sizeof(szSendBuf));
	szSendBuf[0] = 0xF7;
	usSendLen = 1;
	
	szSendBuf[1] = (usTimeOutMs/100)/256;
	szSendBuf[2] = (usTimeOutMs/100)%256;
	usSendLen += 2;
	
	usSendLen += 1;
	iRet = RS232NacTxd1(szSendBuf, usSendLen, 1);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}
	
	memset(szRecvBuf, 0x00, sizeof(szRecvBuf));
	uiTimeOut = 100; //超时100ms
	iRet = RS232NacRxdForSign(szRecvBuf, MAX_PACKET_LEN, uiTimeOut, &usRecvLen);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}
	
	if(memcmp(szRecvBuf, "\xF7\x00",2)==0)
	{
		return 0;
	}
	
	return 1;
}


/*
int ExternSignature(ST_TS_ELEC_SIGNATURE *pstElecSignInfo)//外接手写签名板
{
	return PedSignature(pstElecSignInfo->szTxnInfoCode ,pstElecSignInfo->szImagefileName,
		pstElecSignInfo->ucImageType,pstElecSignInfo->ulTimeout,pstElecSignInfo->szPrompt);
}
*/
/*
int GetModeElecSignature(ST_TS_ELEC_SIGNATURE *pstElecSignInfo,uint8_t SignMode)//SignMode是外接还是内置
{
	if(SignMode == 1)
	{
		return ExternSignature(pstElecSignInfo); //外置
	}
	else 
	{
		return GetElecSignature(pstElecSignInfo);//内接
	}
	
}*/



