#ifndef __DRV_TOUCH_H__
#define __DRV_TOUCH_H__
#include <drv_common.h>
#include <board.h>

#define TOUCH_AD_TOP		251//360//360//160  	//���´������Ķ�����д�� Y ��ģ������ֵ��
#define TOUCH_AD_BOTTOM		3818//3730//3700//3990 	//���´������ĵײ���д�� Y ��ģ������ֵ��
#define TOUCH_AD_LEFT 		3985//3850//3900//160	//���´���������࣬д�� X ��ģ������ֵ��
#define TOUCH_AD_RIGHT		88//180//110//3990		//���´��������Ҳ࣬д�� X ��ģ������ֵ��

#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 
#define TP_READ_TIMES 5    //��ȡ����
#define TP_LOST_TIMES 1	   //�˲�ʱ��TP_READ_TIMES����β���������ݸ���
#define TP_ERR_RANGE 50    //����AD�������Χ 
#define TP_CMD_RDX  0XD0
#define TP_CMD_RDY  0X90
#define TP_SAVE_ADDR_BASE 40
#define TP_SAMP_CNT 7

/*������������*/
struct drv_touch_device
{
	struct rt_device parent;
	
	int16_t xpos;
	int16_t ypos;
	uint16_t cur_xad; 	//��ǰx����ad
	uint16_t cur_yad;	//��ǰy����ad
	uint16_t last_xad; 	//��һ��x����ad
	uint16_t last_yad;	//��һ��y����ad
	uint16_t pen_sta;	//�ʵ�״̬ 

	/*������У׼����(����������ҪУ׼)*/						
	uint16_t xad_left;   //�����X
	uint16_t xad_right;  //���Ҷ�X
	uint16_t yad_top;	 //���Y
	uint16_t yad_bottom; //��׶�Y
};

extern struct drv_touch_device tp_dev;

int drv_touch_hw_init(void);
uint8_t tp_init(void);
uint8_t tp_scan(void);
void read_input_XY(void);

#endif


