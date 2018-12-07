2013/11/26 Lydia
1.修改kbGetString函数，之前的换行之后按清除键会有问题。
2.如果打印已经分配了内存，则prnInit不会重新分配。

2013/11/8 Lydia
1.prnStart函数里面不执行prnClose，应用程序调用prnStart之后还需要调用prnClose释放内存。

2013/10/28 Lydia
1.解决从控制台进有可能连不上的问题。
2.display_create(param.ro_width, 3000, 0, global_font_24, 24);3000*4*384=4.39M,占了4M多的内存，以前的做法是一启动程序就分配这么多内存，等到退出程序的时候才释放，这样导致程序内存占用率很高。现在改成打印的时候才分配内存，打印完成之后就释放，这样程序节省了4.39M的空间，同时运行两个程序是没问题的

2013/10/23 Lydia
1.修正kbGetString函数输入英文字体清屏不能清干净的问题。
2.KEY_INVALID和KEY_TIMEOUT之前的值均为0，为了区分，也为了和8110保持统一,KEY_INVALID值改为0xff,KEY_TIMEOUT的值还是为0

2013/10/8 Lydia
1.3G只能镜像1.0.6版本以及以上才能使用，外面有很多使用1.0.5的版本，如果是1.0.5的版本，修改seos之后gprs也无法使用，所以去掉了3G.

2013/9/27 Lydia
1.增加3G的实现，通过wnet_getmodinfo(&wnet_info);判断机器通讯类型，如果是3G，应用程序的通讯和GPRS是一样的。

2013/09/02
1.WnetInit 去掉wnet_power_off的代码，且wnet_power_on后由4s改成1s


2013/07/11 21：30
1.lcdDisplay的显示根据屏幕大小调整显示
2.修改kbGetHz的函数根据屏幕大小调整参数以便在8110P使用

2013/07/11
1.修改了用宏控制lcdDisplay，及修改lcdClrline

2013/06/24
1.PPPLogin和PPPLogout删除打开modem电源modem_power_control，因为sdk增加了这个接口
2.修改lcdSetFontExt和lcdGetFontExt的接口， 设置字体的同时设置字体高度

2013/06/22
1.修改lcdSetFont的以兼容以前程序
2.修改lcdDisplay会清上一行的线
3.modemPPP拨号不能打开modem的操作，修改了PPPLogin和PPPLogout

20013/06/21
1.修复lcdDisplay出现叠加现象的bug
2.增加lcdSetFontExt和lcdGetFontExt的接口
3.修改lcdSetFont指定设置字体的类型


2013/06/20
1.lcdDisplay去掉subsDurface并且用SetClip设置有效区域;
2.lcdDisplay增加DISP_FLIP的属性，控制局部刷新
3.windowResize函数增加SetClip设置有效区域的操作

2013/06/19
1.prnStart函数增加休眠
2.prnPrintf根据放大倍数决定是否要用放大函数
3.system_display_initialize中window获取创建事件缓冲区失败，将global_events赋空

2013/06/18
1.WnetReset的实现调用wnet_reset
2.增加局部刷新函数lcdFlipLine；

2013/06/09
1.修复fileRemoveDir的bug
2.修复拼音输入__kbGetHZ刚开始“拼音：”显示慢的问题

2013/06/07
1.修改了kbGetString的按键音问题
2.blit问题
3.获取wirless配置问题
4.magSwiped的系统中断问题
5.添加了fileMkDir， fileRemoveDir的函数
6.PPP函数中cdma的设备名

2013/05/27
1.添加创建目录和删除目录的函数

2013/05/20
1.pntDrawPicture的bug,此问题应该也存在7110中

2013/04/09
1.subsurface->flip() 修改成subsurface->Blit();

