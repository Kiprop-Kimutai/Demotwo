2013/11/26 Lydia
1.�޸�kbGetString������֮ǰ�Ļ���֮��������������⡣
2.�����ӡ�Ѿ��������ڴ棬��prnInit�������·��䡣

2013/11/8 Lydia
1.prnStart�������治ִ��prnClose��Ӧ�ó������prnStart֮����Ҫ����prnClose�ͷ��ڴ档

2013/10/28 Lydia
1.����ӿ���̨���п��������ϵ����⡣
2.display_create(param.ro_width, 3000, 0, global_font_24, 24);3000*4*384=4.39M,ռ��4M����ڴ棬��ǰ��������һ��������ͷ�����ô���ڴ棬�ȵ��˳������ʱ����ͷţ��������³����ڴ�ռ���ʺܸߡ����ڸĳɴ�ӡ��ʱ��ŷ����ڴ棬��ӡ���֮����ͷţ����������ʡ��4.39M�Ŀռ䣬ͬʱ��������������û�����

2013/10/23 Lydia
1.����kbGetString��������Ӣ����������������ɾ������⡣
2.KEY_INVALID��KEY_TIMEOUT֮ǰ��ֵ��Ϊ0��Ϊ�����֣�ҲΪ�˺�8110����ͳһ,KEY_INVALIDֵ��Ϊ0xff,KEY_TIMEOUT��ֵ����Ϊ0

2013/10/8 Lydia
1.3Gֻ�ܾ���1.0.6�汾�Լ����ϲ���ʹ�ã������кܶ�ʹ��1.0.5�İ汾�������1.0.5�İ汾���޸�seos֮��gprsҲ�޷�ʹ�ã�����ȥ����3G.

2013/9/27 Lydia
1.����3G��ʵ�֣�ͨ��wnet_getmodinfo(&wnet_info);�жϻ���ͨѶ���ͣ������3G��Ӧ�ó����ͨѶ��GPRS��һ���ġ�

2013/09/02
1.WnetInit ȥ��wnet_power_off�Ĵ��룬��wnet_power_on����4s�ĳ�1s


2013/07/11 21��30
1.lcdDisplay����ʾ������Ļ��С������ʾ
2.�޸�kbGetHz�ĺ���������Ļ��С���������Ա���8110Pʹ��

2013/07/11
1.�޸����ú����lcdDisplay�����޸�lcdClrline

2013/06/24
1.PPPLogin��PPPLogoutɾ����modem��Դmodem_power_control����Ϊsdk����������ӿ�
2.�޸�lcdSetFontExt��lcdGetFontExt�Ľӿڣ� ���������ͬʱ��������߶�

2013/06/22
1.�޸�lcdSetFont���Լ�����ǰ����
2.�޸�lcdDisplay������һ�е���
3.modemPPP���Ų��ܴ�modem�Ĳ������޸���PPPLogin��PPPLogout

20013/06/21
1.�޸�lcdDisplay���ֵ��������bug
2.����lcdSetFontExt��lcdGetFontExt�Ľӿ�
3.�޸�lcdSetFontָ���������������


2013/06/20
1.lcdDisplayȥ��subsDurface������SetClip������Ч����;
2.lcdDisplay����DISP_FLIP�����ԣ����ƾֲ�ˢ��
3.windowResize��������SetClip������Ч����Ĳ���

2013/06/19
1.prnStart������������
2.prnPrintf���ݷŴ��������Ƿ�Ҫ�÷Ŵ���
3.system_display_initialize��window��ȡ�����¼�������ʧ�ܣ���global_events����

2013/06/18
1.WnetReset��ʵ�ֵ���wnet_reset
2.���Ӿֲ�ˢ�º���lcdFlipLine��

2013/06/09
1.�޸�fileRemoveDir��bug
2.�޸�ƴ������__kbGetHZ�տ�ʼ��ƴ��������ʾ��������

2013/06/07
1.�޸���kbGetString�İ���������
2.blit����
3.��ȡwirless��������
4.magSwiped��ϵͳ�ж�����
5.�����fileMkDir�� fileRemoveDir�ĺ���
6.PPP������cdma���豸��

2013/05/27
1.��Ӵ���Ŀ¼��ɾ��Ŀ¼�ĺ���

2013/05/20
1.pntDrawPicture��bug,������Ӧ��Ҳ����7110��

2013/04/09
1.subsurface->flip() �޸ĳ�subsurface->Blit();

