#ifndef _LTDC_H
#define _LTDC_H

//LCD LTDC重要参数集
typedef struct  
{							 
	uint32_t    pwidth;			//LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
	uint32_t    pheight;		//LCD面板的高度,固定参数,不随显示方向改变
	uint16_t    hsw;			//水平同步宽度
	uint16_t    vsw;			//垂直同步宽度
	uint16_t    hbp;			//水平后廊
	uint16_t    vbp;			//垂直后廊
	uint16_t    hfp;			//水平前廊
	uint16_t    vfp;			//垂直前廊 
	uint8_t     activelayer;	//当前层编号:0/1	
	uint8_t     dir;			//0,竖屏;1,横屏;
	uint16_t    width;			//LCD宽度
	uint16_t    height;			//LCD高度
	uint32_t    pixsize;		//每个像素所占字节数
    
}ltdc_dev; 

extern ltdc_dev lcdltdc;		            //管理LCD LTDC参数
extern LTDC_HandleTypeDef LTDC_Handler;	    //LTDC句柄
extern DMA2D_HandleTypeDef DMA2D_Handler;   //DMA2D句柄

#define LCD_PIXEL_FORMAT_ARGB8888       0X00    
#define LCD_PIXEL_FORMAT_RGB888         0X01    
#define LCD_PIXEL_FORMAT_RGB565         0X02       
#define LCD_PIXEL_FORMAT_ARGB1555       0X03      
#define LCD_PIXEL_FORMAT_ARGB4444       0X04     
#define LCD_PIXEL_FORMAT_L8             0X05     
#define LCD_PIXEL_FORMAT_AL44           0X06     
#define LCD_PIXEL_FORMAT_AL88           0X07      

///////////////////////////////////////////////////////////////////////
//用户修改配置部分:
#define EXT_SDRAM_ADDR			0xC0000000		//使用FMC_SDRAM_BANK1,也就是BANK5，地址映射到 0xC0000000
									//FMC_SDRAM_BANK2,也就是BANK6，地址映射到 0xD0000000
//定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
//定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR			0X00000000	
//LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR			EXT_SDRAM_ADDR  

void LTDC_Switch(uint8_t sw);					//LTDC开关
void LTDC_Layer_Switch(uint8_t layerx,uint8_t sw);	//层开关
void LTDC_Select_Layer(uint8_t layerx);			//层选择
void LTDC_Display_Dir(uint8_t dir);				//显示方向控制
void LTDC_Draw_Point(uint16_t x,uint16_t y,uint32_t color);//画点函数
uint32_t LTDC_Read_Point(uint16_t x,uint16_t y);			//读点函数
void LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);			//矩形单色填充函数
void LTDC_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);	//矩形彩色填充函数
void LTDC_Clear(uint32_t color);					//清屏函数
uint8_t LTDC_Clk_Set(uint32_t pllsain,uint32_t pllsair,uint32_t pllsaidivr);//LTDC时钟配置
void LTDC_Layer_Window_Config(uint8_t layerx,uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);//LTDC层窗口设置
void LTDC_Layer_Parameter_Config(uint8_t layerx,uint32_t bufaddr,uint8_t pixformat,uint8_t alpha,uint8_t alpha0,uint8_t bfac1,uint8_t bfac2,uint32_t bkcolor);//LTDC基本参数设置
uint16_t LTDC_PanelID_Read(void);				//LCD ID读取函数
void LTDC_User_Init(void);						//LTDC初始化函数

#endif 
