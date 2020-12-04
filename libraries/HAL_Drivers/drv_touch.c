#include "board.h"
#include "drv_touch.h"

#define TOUCH_TDIN_H() HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET)
#define TOUCH_TDIN_L() HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET)
#define TOUCH_TCLK_H() HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET)
#define TOUCH_TCLK_L() HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET)
#define TOUCH_TCS_H()  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_8, GPIO_PIN_SET)
#define TOUCH_TCS_L()  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_8, GPIO_PIN_RESET)
#define TOUCH_READ_PEN() 	HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)
#define TOUCH_READ_DOUT() 	HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_3)

struct drv_touch_device tp_dev;

uint8_t tp_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_3;//sda
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin  = GPIO_PIN_8;//cs
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin  = GPIO_PIN_6;//sck
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin  = GPIO_PIN_7;//pen
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin  = GPIO_PIN_3;//dout
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	tp_dev.xad_left = TOUCH_AD_LEFT;
	tp_dev.xad_right = TOUCH_AD_RIGHT;
	tp_dev.yad_top = TOUCH_AD_TOP;
	tp_dev.yad_bottom = TOUCH_AD_BOTTOM;

	return 1; 	
}

static void tp_delay_us(int delay)
{
	for(int i = 0;i < (delay * 5 * 2); i++);
}

void tp_write_byte(uint8_t num)    
{  
	uint8_t count = 0; 
        
	for(count = 0; count < 8; count++)  
	{ 	  
		if((num&0x80) == 0x80)
			TOUCH_TDIN_H();
		else
			TOUCH_TDIN_L();   
		
		num <<= 1;    
                
		TOUCH_TCLK_L();    
		tp_delay_us(1);   
		TOUCH_TCLK_H();		//上升沿有效	        
	}		 			    
} 		 
	   
uint16_t tp_read_ad(uint8_t cmd)	  
{ 	 
	uint8_t count = 0; 	  
	uint16_t  num = 0; 
        
	TOUCH_TCLK_L();		//先拉低时钟 	 
	TOUCH_TDIN_L(); 	//拉低数据线
	TOUCH_TCS_L(); 		//选中触摸屏IC
        
	tp_write_byte(cmd); //发送命令字
	tp_delay_us(6);  
	TOUCH_TCLK_L(); 
	tp_delay_us(1);    
	TOUCH_TCLK_H();	
	tp_delay_us(1);           
	TOUCH_TCLK_L();	        
	for(count = 0; count < 16; count++)//读出16位数据,只有高12位有效 
	{ 				  
		num <<= 1; 	 
                
		TOUCH_TCLK_L();	//下降沿有效  	                
		tp_delay_us(1);                    
 		TOUCH_TCLK_H();
                
 		if(TOUCH_READ_DOUT() == 1)
			num++;
	}  	
        
	num >>= 4;   	//只有高12位有效.       
	TOUCH_TCS_H();  //释放片选	
        
	return num;   
}

uint16_t tp_read_ad_filter(uint8_t cmd)
{
	uint16_t  i, j;
	uint16_t  buf[TP_READ_TIMES];
	uint16_t  sum = 0;
	uint16_t  temp;
        
	for(i = 0; i < TP_READ_TIMES; i++)
	{
		buf[i] = tp_read_ad(cmd);	
	}
        
	for(i = 0; i < (TP_READ_TIMES - 1); i++)//排序
	{
		for(j = i + 1; j < TP_READ_TIMES; j++)
		{
			if(buf[i] > buf[j])//升序排列
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}	  
	
	sum = 0;	
	for(i = TP_LOST_TIMES; i < (TP_READ_TIMES - TP_LOST_TIMES); i++)
	{
		sum += buf[i];
	}
	
	temp = sum / (TP_READ_TIMES - 2 * TP_LOST_TIMES);
	
	return temp;   
} 

uint8_t tp_read_xyad(uint16_t *xad, uint16_t *yad)
{
	uint16_t xtemp,ytemp;		
        
	xtemp = tp_read_ad_filter(TP_CMD_RDX);
	ytemp = tp_read_ad_filter(TP_CMD_RDY);	  												   

	*xad = xtemp;
	*yad = ytemp;
        
	return 1; 
}

uint8_t tp_read_xyad_times(uint16_t *x, uint16_t *y) 
{
	static uint16_t x1,y1;
	static uint16_t x2,y2;  
        
	tp_read_xyad(&x1, &y1);   
	tp_read_xyad(&x2, &y2);	   
        
	if((abs( x1 - x2) < TP_ERR_RANGE) && (abs(y1 - y2) < TP_ERR_RANGE)) 
	{
		*x = (x1 + x2) / 2;
		*y = (y1 + y2) / 2;                
		return 1;
	}
	else 
	{
		return 0;	
	}
}

uint8_t tp_scan(void)
{			   
	if(TOUCH_READ_PEN() == 0)//有按键按下
	{
		tp_read_xyad_times(&tp_dev.cur_xad, &tp_dev.cur_yad);//读取物理坐标
		
		if((tp_dev.pen_sta & TP_PRES_DOWN) == 0)//之前没有被按下
		{		 
			tp_dev.pen_sta = TP_PRES_DOWN | TP_CATH_PRES;//按键按下                        
			tp_dev.last_xad = tp_dev.cur_xad;
			tp_dev.last_yad = tp_dev.cur_yad;  	   			 
		}

		//AD值转换成坐标
		tp_dev.xpos = (tp_dev.xad_left - tp_dev.cur_xad) * 800 / (tp_dev.xad_left - tp_dev.xad_right);
		tp_dev.ypos = 480 - (tp_dev.yad_bottom - tp_dev.cur_yad) * 480 / (tp_dev.yad_bottom - tp_dev.yad_top);
		if (tp_dev.xpos < 0) tp_dev.xpos = 0;
		if (tp_dev.ypos < 0) tp_dev.ypos = 0;
	}
	else
	{
		if((tp_dev.pen_sta & TP_PRES_DOWN) ==  TP_PRES_DOWN)   //之前是被按下的
		{
			tp_dev.pen_sta &= ~(1 << 7);      //标记按键松开	
		}
		else//之前就没有被按下
		{
			tp_dev.last_xad = 0;
			tp_dev.last_yad = 0;
			tp_dev.cur_xad = 0xFFFF;
			tp_dev.cur_yad = 0xFFFF;
			tp_dev.xpos = 0;
			tp_dev.ypos = 0;
		}	    
	}
        
	return tp_dev.pen_sta & TP_PRES_DOWN;//返回当前的触屏状态
}  									    

static void bublesore(uint16_t *inbuf,uint16_t inlen)
{
	uint16_t i,j;
	uint16_t temp;
	for(j = 0;j < (inlen-10); j++)
	{
		for(i = 0; i < (inlen-j-1); i++)
		{
			if(inbuf[i] > inbuf[i+1]) 
			{
				temp = inbuf[i];
				inbuf[i] = inbuf[i+1];
				inbuf[i+1] = temp;
			}
		}
	}
}

static uint16_t average_fun(uint16_t *inbuf,uint16_t inlen,uint8_t start_v,uint8_t end_v)
{
	uint32_t ltemp = 0;
	uint16_t i;
	
	for(i = start_v; i < (inlen-end_v); i++)
	{
		ltemp += inbuf[i];
	}
	
	return(ltemp / (inlen - (start_v + end_v)));
}

/* 数值过滤 主要用途防止飞点出现 */
static uint16_t median_filter_fun(uint16_t *inbuf,uint16_t inlen,uint8_t start_v,uint8_t end_v)
{
	uint16_t itemp;
	
	bublesore(inbuf,inlen);
	
	if(inbuf[0] + 77 < inbuf[inlen - 1]){	//最大值与最小值差不可超过30
		return 0;
	}
	
	itemp = average_fun(inbuf,inlen,start_v,end_v);
	
	return itemp;	
}

void read_input_XY(void)
{ 
	uint16_t tbuf_x[TP_SAMP_CNT];
	uint16_t tbuf_y[TP_SAMP_CNT];
	uint8_t i,try_cnt = 0;

	if(TOUCH_READ_PEN() == 0)//有按键按下
	{
		do
		{
			try_cnt++;
			for(i = 0; i < TP_SAMP_CNT; i++)
			{
				tbuf_x[i] = tp_read_ad(TP_CMD_RDX); 
				tbuf_y[i] = tp_read_ad(TP_CMD_RDY);
			}

			tp_dev.cur_xad = median_filter_fun(tbuf_x,TP_SAMP_CNT,2,2);
			tp_dev.cur_yad = median_filter_fun(tbuf_y,TP_SAMP_CNT,2,2);
		}
		while(((tp_dev.cur_xad == 0)||(tp_dev.cur_yad == 0))&&(try_cnt < 100));	
		
		//AD值转换成坐标
		tp_dev.xpos = (tp_dev.xad_left - tp_dev.cur_xad) * 800 / (tp_dev.xad_left - tp_dev.xad_right);
		tp_dev.ypos = 480 - (tp_dev.yad_bottom - tp_dev.cur_yad) * 480 / (tp_dev.yad_bottom - tp_dev.yad_top);
		if (tp_dev.xpos < 0) tp_dev.xpos = 0;
		if (tp_dev.ypos < 0) tp_dev.ypos = 0;
	} 
	else
	{
		tp_dev.cur_xad = 0xFFFF;
		tp_dev.cur_yad = 0xFFFF;
		tp_dev.xpos = 0;
		tp_dev.ypos = 0;
	}
}
			
static rt_err_t drv_touch_init(struct rt_device *device)
{
    tp_init();
    
    return RT_EOK;
}

static rt_err_t drv_touch_control(struct rt_device *device, int cmd, void *args)
{

    return RT_EOK;
}


#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops touch_ops =
{
	drv_touch_init,
	RT_NULL,
	RT_NULL,
	RT_NULL,
	RT_NULL,
	drv_touch_control
};
#endif

int drv_touch_hw_init(void)
{
    rt_err_t result = RT_EOK;
    struct rt_device *device = &tp_dev.parent;
	
    /* memset tp_dev to zero */
    rt_memset(&tp_dev, 0x00, sizeof(struct drv_touch_device));

    device->type = RT_Device_Class_Touch;
    
#ifdef RT_USING_DEVICE_OPS
    device->ops = &touch_ops;
#else
    device->init = drv_touch_init;
    device->control = drv_touch_control;
#endif
    
    /* register lcd device */
    rt_device_register(device, "touch", RT_DEVICE_FLAG_RDWR);

    return result;
}
INIT_DEVICE_EXPORT(drv_touch_hw_init);
