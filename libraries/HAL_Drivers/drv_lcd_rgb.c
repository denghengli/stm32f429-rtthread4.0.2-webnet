#include <board.h>
#include "drv_lcd_rgb.h"
#include "drv_ltdc.h"
#include "string.h"

//#define DRV_DEBUG
#define LOG_TAG "drv.lcd"
#include <drv_log.h>

#define LCD_DEVICE(dev) (struct drv_lcd_device *)(dev)

struct drv_lcd_device
{
    struct rt_device parent;

    struct rt_device_graphic_info lcd_info;
};

lcd_dev lcddev;
static struct drv_lcd_device _lcd;
  
uint32_t POINT_COLOR = 0xFF000000;		//画笔颜色
uint32_t BACK_COLOR = 0xFFFFFFFF;  	//背景色 

static void delay_us(int delay)
{
	for(int i = 0; i < delay * 5; i++);
}

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(uint16_t regval)
{   
	regval = regval;		//使用-O2优化的时候,必须插入的延时
	LCD->REG = regval;//写入要写的寄存器序号	 
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(uint16_t data)
{	  
	data=data;			//使用-O2优化的时候,必须插入的延时
	LCD->RAM = data;		 
}
//读LCD数据
//返回值:读到的值
uint16_t LCD_RD_DATA(void)
{
	uint16_t ram;			//防止被优化
	ram = LCD->RAM;	
	return ram;	 
}					   
//写寄存器
//REG:寄存器地址
//REGValue:要写入的数据
void LCD_WriteReg(uint16_t REG,uint16_t REGValue)
{	
	LCD->REG = REG;		//写入要写的寄存器序号	 
	LCD->RAM = REGValue;//写入数据	    		 
}	   
//读寄存器
//REG:寄存器地址
//返回值:读到的数据
uint16_t LCD_ReadReg(uint16_t REG)
{										   
	LCD_WR_REG(REG);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->REG = lcddev.wramcmd;	  
}	 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(uint16_t RGB_Code)
{							    
	LCD->RAM = RGB_Code;//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
uint16_t LCD_BGR2RGB(uint16_t c)
{
	uint16_t  r,g,b,rgb;   
	b = (c>>0)&0x1f;
	g = (c>>5)&0x3f;
	r = (c>>11)&0x1f;	 
	rgb = (b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(uint8_t i)
{
	while(i--);
}
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
uint32_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
 	uint16_t r = 0,g = 0,b = 0;
	if(x>=lcddev.width||y >= lcddev.height)
	{
		return 0;	//超过了范围,直接返回
	}
	if(lcdltdc.pwidth != 0)							//如果是RGB屏
	{
		return LTDC_Read_Point(x,y);
	}
	LCD_SetCursor(x,y);	    
	if((lcddev.id==0X9341)||(lcddev.id==0X5310)|| (lcddev.id==0X1963) )
	{
		LCD_WR_REG(0X2E);//9341/3510/1963 发送读GRAM指令
	}
	else if(lcddev.id == 0X5510)
	{
		LCD_WR_REG(0X2E00);	//5510 发送读GRAM指令
	}
 	r = LCD_RD_DATA();								//dummy Read	   
	if(lcddev.id == 0X1963)
	{
		return r;					//1963直接读就可以 
	}
	opt_delay(2);	  
 	r = LCD_RD_DATA();  		  						//实际坐标颜色
	//9341/NT35310/NT35510要分2次读出 
	opt_delay(2);	  
	b = LCD_RD_DATA(); 
	g = r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
	g <<= 8; 
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341/NT35310/NT35510需要公式转换一下
}			 
//LCD开启显示
void LCD_DisplayOn(void)
{					   
	if(lcdltdc.pwidth != 0)
	{
		LTDC_Switch(1);//开启LCD
	}
	else if((lcddev.id == 0X9341) || (lcddev.id == 0X5310) || (lcddev.id == 0X1963))
	{
		LCD_WR_REG(0X29);	//开启显示
	}
	else if(lcddev.id == 0X5510)
	{
		LCD_WR_REG(0X2900);	//开启显示
	}
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	if(lcdltdc.pwidth != 0)
	{
		LTDC_Switch(0);//关闭LCD
	}
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X1963)
	{
		LCD_WR_REG(0X28);	//关闭显示
	}
	else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(0X2800);	//关闭显示
	}
}   
//设置光标位置(对RGB屏无效)
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	 
 	if( (lcddev.id == 0X9341)||(lcddev.id == 0X5310))
	{		
		
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);
		LCD_WR_DATA(Xpos&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);
		LCD_WR_DATA(Ypos&0XFF); 
		
	}
	else if(lcddev.id == 0X1963)
	{  			 		
		if(lcddev.dir == 0)//x坐标需要变换
		{
			Xpos=lcddev.width-1-Xpos;
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0); 		
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF);		 	 
		}
		else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(Xpos>>8);
			LCD_WR_DATA(Xpos&0XFF); 		
			LCD_WR_DATA((lcddev.width-1)>>8);
			LCD_WR_DATA((lcddev.width-1)&0XFF);		 	 			
		}	
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);
		LCD_WR_DATA(Ypos&0XFF); 		
		LCD_WR_DATA((lcddev.height-1)>>8);
		LCD_WR_DATA((lcddev.height-1)&0XFF); 			 		
		
	}
	else if(lcddev.id == 0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos>>8); 		
		LCD_WR_REG(lcddev.setxcmd+1);
		LCD_WR_DATA(Xpos&0XFF);			 
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos>>8);  		
		LCD_WR_REG(lcddev.setycmd+1);
		LCD_WR_DATA(Ypos&0XFF);			
	} 
} 		 
//设置LCD的自动扫描方向(对RGB屏无效)
//注意:其他函数可能会受到此函数设置的影响(尤其是9341),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9341/5310/5510/1963等IC已经实际测试	   	   
void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;  
	if(( (lcddev.dir==1) && (lcddev.id!=0X1963))||( (lcddev.dir==0)&&(lcddev.id==0X1963)))//横屏时，对1963不改变扫描方向！竖屏时1963改变方向
	{			   
		switch(dir)//方向转换
		{
			case 0:
				dir=6;
				break;
			case 1:
				dir=7;
				break;
			case 2:
				dir=4;
				break;
			case 3:
				dir=5;
				break;
			case 4:
				dir=1;
				break;
			case 5:
				dir=0;
				break;
			case 6:
				dir=3;
				break;
			case 7:
				dir=2;
				break;	     
		}
	} 
	if((lcddev.id == 0x9341)||(lcddev.id == 0X5310)||(lcddev.id == 0X5510)||(lcddev.id == 0X1963))//9341/5310/5510/1963,特殊处理
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id == 0X5510)
		{
			dirreg = 0X3600;
		}
		else 
		{
			dirreg = 0X36;
		}
		
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510)&&(lcddev.id!=0X1963))
		{
			regval|=0X08;//5310/5510/1963不需要BGR 
		}
		
 		LCD_WriteReg(dirreg,regval);
		
		if(lcddev.id != 0X1963)//1963不做坐标处理
		{
			if(regval&0X20)
			{
				if(lcddev.width < lcddev.height)//交换X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}
			else  
			{
				if(lcddev.width > lcddev.height)//交换X,Y
				{
					temp = lcddev.width;
					lcddev.width = lcddev.height;
					lcddev.height = temp;
				}
			}  
		}
		
		if(lcddev.id == 0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);
			LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);
			LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);
			LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);
			LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);
			LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);
			LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);
			LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);
			LCD_WR_DATA((lcddev.height-1)&0XFF);
		}
		else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);
			LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);
			LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	} 
}     
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	if(lcdltdc.pwidth!=0)//如果是RGB屏
	{
		LTDC_Draw_Point(x,y,POINT_COLOR);
	}
	else
	{
		LCD_SetCursor(x,y);		//设置光标位置 
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		LCD->RAM=POINT_COLOR; 
	}
}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
{	   
	if(lcdltdc.pwidth!=0)//如果是RGB屏
	{
		LTDC_Draw_Point(x,y,color);
		return;
	}
	else if((lcddev.id == 0X9341)||(lcddev.id == 0X5310))
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		 	 
	}
	else if(lcddev.id == 0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(x>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(x&0XFF);	  
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(y>>8);  
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(y&0XFF); 
	}
	else if(lcddev.id == 0X1963)
	{
		if(lcddev.dir == 0)
		{
			x = lcddev.width-1-x;
		}
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);
		LCD_WR_DATA(x&0XFF); 		
		LCD_WR_DATA(x>>8);
		LCD_WR_DATA(x&0XFF); 		
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);
		LCD_WR_DATA(y&0XFF); 		
		LCD_WR_DATA(y>>8);
		LCD_WR_DATA(y&0XFF); 		
	}		 
	LCD->REG = lcddev.wramcmd; 
	LCD->RAM = color; 
}	  
//SSD1963 背光设置
//pwm:背光等级,0~100.越大越亮.
void LCD_SSD_BackLightSet(uint8_t pwm)
{	
	LCD_WR_REG(0xBE);	//配置PWM输出
	LCD_WR_DATA(0x05);	//1设置PWM频率
	LCD_WR_DATA((uint16_t)(pwm*2.55));//2设置PWM占空比
	LCD_WR_DATA(0x01);	//3设置C
	LCD_WR_DATA(0xFF);	//4设置D
	LCD_WR_DATA(0x00);	//5设置E
	LCD_WR_DATA(0x00);	//6设置F
}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir)
{
    lcddev.dir = dir;         //横屏/竖屏
	if(lcdltdc.pwidth != 0)   //如果是RGB屏
	{
		LTDC_Display_Dir(dir);
		lcddev.width = lcdltdc.width;
		lcddev.height = lcdltdc.height;
		return;
	}
	if(dir == 0)			//竖屏
	{
		lcddev.width = 240;
		lcddev.height = 320;
		if((lcddev.id == 0X9341)||(lcddev.id == 0X5310))
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id == 0X5310)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}
		else if(lcddev.id == 0x5510)
		{
			lcddev.wramcmd = 0X2C00;
	 		lcddev.setxcmd = 0X2A00;
			lcddev.setycmd = 0X2B00; 
			lcddev.width = 480;
			lcddev.height = 800;
		}
		else if(lcddev.id == 0X1963)
		{
			lcddev.wramcmd = 0X2C;	//设置写入GRAM的指令 
			lcddev.setxcmd = 0X2B;	//设置写X坐标指令
			lcddev.setycmd = 0X2A;	//设置写Y坐标指令
			lcddev.width = 480;		//设置宽度480
			lcddev.height = 800;		//设置高度800  
		}
	}
	else 				//横屏
	{	  				
		lcddev.width = 320;
		lcddev.height = 240;
		if( (lcddev.id == 0X9341)||(lcddev.id == 0X5310))
		{
			lcddev.wramcmd = 0X2C;
	 		lcddev.setxcmd = 0X2A;
			lcddev.setycmd = 0X2B;  	 
		}
		else if(lcddev.id == 0x5510)
		{
			lcddev.wramcmd = 0X2C00;
	 		lcddev.setxcmd = 0X2A00;
			lcddev.setycmd = 0X2B00; 
			lcddev.width = 800;
			lcddev.height = 480;
		}
		else if(lcddev.id == 0X1963)
		{
			lcddev.wramcmd = 0X2C;	//设置写入GRAM的指令 
			lcddev.setxcmd = 0X2A;	//设置写X坐标指令
			lcddev.setycmd = 0X2B;	//设置写Y坐标指令
			lcddev.width = 800;		//设置宽度800
			lcddev.height = 480;		//设置高度480  
		}
		if(lcddev.id == 0X5310)
		{ 	 
			lcddev.width = 480;
			lcddev.height = 320; 			
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}	 
//设置窗口(对RGB屏无效),并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{     
	uint16_t twidth,theight;
	twidth = sx+width-1;
	theight = sy+height-1;
	
	if( (lcddev.id==0X9341) || (lcddev.id==0X5310) ||( (lcddev.dir==1) && (lcddev.id==0X1963)))
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA(twidth>>8); 
		LCD_WR_DATA(twidth&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(theight>>8); 
		LCD_WR_DATA(theight&0XFF); 
	}
	else if(lcddev.id == 0X1963)//1963竖屏特殊处理
	{
		sx = lcddev.width-width-sx; 
		height = sy+height-1; 
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA((sx+width-1)>>8); 
		LCD_WR_DATA((sx+width-1)&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(height>>8); 
		LCD_WR_DATA(height&0XFF); 		
	}
	else if(lcddev.id == 0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(sx>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);
		LCD_WR_DATA(sx&0XFF);	  
		LCD_WR_REG(lcddev.setxcmd+2);
		LCD_WR_DATA(twidth>>8);   
		LCD_WR_REG(lcddev.setxcmd+3);
		LCD_WR_DATA(twidth&0XFF);   
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(sy>>8);   
		LCD_WR_REG(lcddev.setycmd+1);
		LCD_WR_DATA(sy&0XFF);  
		LCD_WR_REG(lcddev.setycmd+2);
		LCD_WR_DATA(theight>>8);   
		LCD_WR_REG(lcddev.setycmd+3);
		LCD_WR_DATA(theight&0XFF);  
	} 
}

//SRAM底层驱动，时钟使能，引脚分配
//此函数会被HAL_SRAM_Init()调用
//hsram:SRAM句柄
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_FMC_CLK_ENABLE();				//使能FMC时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();			//使能GPIOD时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();			//使能GPIOE时钟
	
	//初始化PD0,1,4,5,7,8,9,10,13,14,15
	GPIO_Initure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|\
                     GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode = GPIO_MODE_AF_PP; 		//复用
	GPIO_Initure.Pull = GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;		//高速
	GPIO_Initure.Alternate = GPIO_AF12_FMC;	//复用为FMC
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	//初始化PE7,8,9,10,11,12,13,14,15
	GPIO_Initure.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}
        
//初始化lcd
//该初始化函数可以初始化各种型号的LCD(详见本.c文件最前面的描述)
void LCD_Init(void)
{ 	  
	lcddev.dir = 1;	//横屏
	lcddev.width = 800;
	lcddev.height = 480;
	lcddev.id = LTDC_PanelID_Read();	//检查是否有RGB屏接入

	LTDC_User_Init();			    //ID非零,说明有RGB屏接入.

	LCD_Display_Dir(1);		//默认为横屏
    LCD_BL_INIT;
	LCD_BL_ON;				//点亮背光
	LCD_Clear(WHITE);
}  

//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint32_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint = lcddev.width;
        
	if(lcdltdc.pwidth != 0)	//如果是RGB屏
	{
		LTDC_Clear(color);
	}
	else
	{
		totalpoint *= lcddev.height; 		//得到总点数
		LCD_SetCursor(0x00,0x0000);		//设置光标位置 
		LCD_WriteRAM_Prepare();     		//开始写入GRAM	 	  
		for(index = 0; index < totalpoint; index++)
		{
			LCD->RAM = color;	
		}
	}
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	if(lcdltdc.pwidth != 0)	//如果是RGB屏
	{
		LTDC_Fill(sx,sy,ex,ey,color);
	}else
	{	
		xlen = ex-sx+1;	 
		for(i = sy; i <= ey; i++)
		{
			LCD_SetCursor(sx,i);      				//设置光标位置 
			LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
			for(j = 0; j < xlen; j++)
			{
				LCD->RAM = color;	//显示颜色 	    
			}
		} 
	}
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	if(lcdltdc.pwidth != 0)	//如果是RGB屏
	{
		LTDC_Color_Fill(sx,sy,ex,ey,color);
	}else
	{
		width = ex-sx+1; 			//得到填充的宽度
		height = ey-sy+1;			//高度
		for(i = 0; i < height; i++)
		{
			LCD_SetCursor(sx,sy+i);   	//设置光标位置 
			LCD_WriteRAM_Prepare();     //开始写入GRAM
			for(j = 0; j < width; j++)
			{
				LCD->RAM = color[i*width+j];//写入数据 
			}
		}
	}	
}  

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x = x2-x1; //计算坐标增量 
	delta_y = y2-y1; 
	uRow = x1; 
	uCol = y1; 
	if(delta_x > 0)
	{
		incx=1; //设置单步方向 
	}
	else if(delta_x==0)
	{
		incx=0;//垂直线 
	}
	else 
	{
		incx=-1;
		delta_x=-delta_x;
	} 
	if(delta_y>0)
	{
		incy=1; 
	}
	else if(delta_y==0)
	{
		incy=0;//水平线 
	}
	else
	{
		incy=-1;
		delta_y=-delta_y;
	} 
	if( delta_x>delta_y)
	{
		distance=delta_x; //选取基本增量坐标轴 
	}
	else 
	{
		distance=delta_y; 
	}
	for(t = 0; t <= (distance+1); t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr += delta_x ; 
		yerr += delta_y ; 
		if(xerr > distance) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if(yerr > distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a = 0; 
	b = r;	  
	di = 3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di < 0)
		{
			di += 4*a+6;	 
		}
		else
		{
			di += 10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
//    uint8_t temp,t1,t;
//	uint16_t y0 = y;
//	uint8_t csize = (size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
// 	num = num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
//	for(t = 0; t < csize; t++)
//	{   
//		if(size == 12)
//		{
//			temp = asc2_1206[num][t]; 	 	//调用1206字体
//		}
//		else if(size == 16)
//		{
//			temp = asc2_1608[num][t];	//调用1608字体
//		}
//		else if(size == 24)
//		{
//			temp = asc2_2412[num][t];	//调用2412字体
//		}
//		else if(size == 32)
//		{
//			temp = asc2_3216[num][t];	//调用3216字体
//		}
//		else 
//		{
//			return;								//没有的字库
//		}
//		for(t1 = 0;t1 < 8; t1++)
//		{			    
//			if(temp&0x80)
//			{
//				LCD_Fast_DrawPoint(x,y,POINT_COLOR);
//			}
//			else if(mode==0)
//			{
//				LCD_Fast_DrawPoint(x,y,BACK_COLOR);
//			}
//			temp <<= 1;
//			y++;
//			if(y >= lcddev.height)
//			{
//				return;		//超区域了
//			}
//			if((y-y0) == size)
//			{
//				y = y0;
//				x++;
//				if(x >= lcddev.width)
//				{
//					return;	//超区域了
//				}
//				break;
//			}
//		}  	 
//	}  	    	   	 	  
}   

//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	
	while(n--)
	{
		result*=m;    
	}
	
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow = 0;						   
	for(t = 0; t < len; t++)
	{
		temp = (num/LCD_Pow(10,len-t-1))%10;
		if( (enshow == 0) && (t<(len-1)))
		{
			if(temp == 0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}
			else 
			{
				enshow = 1; 
			}
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	
	uint8_t enshow = 0;
	
	for(t = 0; t < len; t++)
	{
		temp = (num/LCD_Pow(10,len-t-1))%10;
		if(enshow == 0&&t<(len-1))
		{
			if(temp == 0)
			{
				if(mode&0X80)
				{
					LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				}
				else 
				{
					LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
				}
 				continue;
			}
			else 
			{
				enshow=1;
			}
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x >= width)
	  {
		  x = x0;
		  y += size;
	  }
        if(y >= height)
	  {
		  break;//退出
	  }
        LCD_ShowChar(x,y,*p,size,0);
        x += size/2;
        p++;
    }  
}

static rt_err_t drv_lcd_init(struct rt_device *device)
{
    LCD_Init();
    
    return RT_EOK;
}

static rt_err_t drv_lcd_control(struct rt_device *device, int cmd, void *args)
{
    struct drv_lcd_device *lcd = LCD_DEVICE(device);
    
    switch (cmd)
    {
        case RTGRAPHIC_CTRL_GET_INFO:
        {
            struct rt_device_graphic_info *info = (struct rt_device_graphic_info *)args;

            RT_ASSERT(info != RT_NULL);
            
            //this needs to be replaced by the customer
            info->pixel_format  = lcd->lcd_info.pixel_format;
            info->bits_per_pixel = lcd->lcd_info.bits_per_pixel;
            info->width = lcddev.width;
            info->height = lcddev.height;
        }
        break;
    }

    return RT_EOK;
}


#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops lcd_ops =
{
        drv_lcd_init,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        drv_lcd_control};
#endif

int drv_lcd_hw_init(void)
{
    rt_err_t result = RT_EOK;
    struct rt_device *device = &_lcd.parent;
    /* memset _lcd to zero */
    memset(&_lcd, 0x00, sizeof(_lcd));

    _lcd.lcd_info.bits_per_pixel = 16;
    _lcd.lcd_info.pixel_format = RTGRAPHIC_PIXEL_FORMAT_RGB565;

    device->type = RT_Device_Class_Graphic;
    
#ifdef RT_USING_DEVICE_OPS
    device->ops = &lcd_ops;
#else
    device->init = drv_lcd_init;
    device->control = drv_lcd_control;
#endif

    //device->user_data = &fsmc_lcd_ops;
    
    /* register lcd device */
    rt_device_register(device, "rgblcd", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);

    return result;
}
INIT_DEVICE_EXPORT(drv_lcd_hw_init);


