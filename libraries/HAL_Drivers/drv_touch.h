#ifndef __DRV_TOUCH_H__
#define __DRV_TOUCH_H__
#include <drv_common.h>
#include <board.h>

#define TOUCH_AD_TOP		251//360//360//160  	//按下触摸屏的顶部，写下 Y 轴模拟输入值。
#define TOUCH_AD_BOTTOM		3818//3730//3700//3990 	//按下触摸屏的底部，写下 Y 轴模拟输入值。
#define TOUCH_AD_LEFT 		3985//3850//3900//160	//按下触摸屏的左侧，写下 X 轴模拟输入值。
#define TOUCH_AD_RIGHT		88//180//110//3990		//按下触摸屏的右侧，写下 X 轴模拟输入值。

#define TP_PRES_DOWN 0x80  //触屏被按下	  
#define TP_CATH_PRES 0x40  //有按键按下了 
#define TP_READ_TIMES 5    //读取次数
#define TP_LOST_TIMES 1	   //滤波时在TP_READ_TIMES中首尾丢弃的数据个数
#define TP_ERR_RANGE 50    //两次AD允许的误差范围 
#define TP_CMD_RDX  0XD0
#define TP_CMD_RDY  0X90
#define TP_SAVE_ADDR_BASE 40
#define TP_SAMP_CNT 7

/*触摸屏控制器*/
struct drv_touch_device
{
	struct rt_device parent;
	
	int16_t xpos;
	int16_t ypos;
	uint16_t cur_xad; 	//当前x坐标ad
	uint16_t cur_yad;	//当前y坐标ad
	uint16_t last_xad; 	//上一次x坐标ad
	uint16_t last_yad;	//上一次y坐标ad
	uint16_t pen_sta;	//笔的状态 

	/*触摸屏校准参数(电容屏不需要校准)*/						
	uint16_t xad_left;   //最左端X
	uint16_t xad_right;  //最右端X
	uint16_t yad_top;	 //最顶端Y
	uint16_t yad_bottom; //最底端Y
};

extern struct drv_touch_device tp_dev;

int drv_touch_hw_init(void);
uint8_t tp_init(void);
uint8_t tp_scan(void);
void read_input_XY(void);

#endif


