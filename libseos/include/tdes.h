/****************************************************************************
NAME
	tdes.h - hash����

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
*		  ��ƽ��		  2007-09-12		     ����			01-01-01
****************************************************************************/

#ifndef POS_TDES_H
#define POS_TDES_H

#include <stdint.h>

#ifndef  ERROR
#define  ERROR					0x01
#endif

#ifndef TDEA_NONE
#define TDEA_NONE                  0x02            // none
#endif

#ifndef TDEA_ENCRYPT
#define TDEA_ENCRYPT            0x01            // TDEA encryption
#endif

#ifndef TDEA_DECRYPT
#define TDEA_DECRYPT            0x00            // TDEA decryption
#endif

void Des(const void *source, uint8_t *dest, const void *key, uint8_t flag);


/**************************************************************************
* ��������:
*		   int  TDEA(uchar *DataIn, uchar *DataOut, uchar *DesKey, uint KeyLen, uint Mode)
* ��������:
*		  ʵ��Tripe DES ����
* �������:
*		  DataIn          ������TDES���������(8�ֽ�)
*              DesKey         DES��Կ
*              KeyLen         ��Կ����(8��16��24���ֿ���ֵ)
*              Mode            ���ܻ��߽�������
* �������:
*		  DataOut        TDES�����õ�������(8�ֽ�)
* ����ֵ:
*		  0                  ����ɹ�
*             0x01              �����������(��Կ���ȴ�����߲���ģʽ����)
* ��ʷ��¼:
*		   �޸���		�޸�����	�޸�����		�汾��
*		  ��ƽ��		  2007-09-12		     ����			01-01-01
****************************************************************************/
int TDEA(const void *DataIn, void *DataOut, const void *DesKey, uint32_t KeyLen, uint32_t Mode);

#endif
