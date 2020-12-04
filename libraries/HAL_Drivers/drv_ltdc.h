#ifndef _LTDC_H
#define _LTDC_H

//LCD LTDC��Ҫ������
typedef struct  
{							 
	uint32_t    pwidth;			//LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������
	uint32_t    pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	uint16_t    hsw;			//ˮƽͬ�����
	uint16_t    vsw;			//��ֱͬ�����
	uint16_t    hbp;			//ˮƽ����
	uint16_t    vbp;			//��ֱ����
	uint16_t    hfp;			//ˮƽǰ��
	uint16_t    vfp;			//��ֱǰ�� 
	uint8_t     activelayer;	//��ǰ����:0/1	
	uint8_t     dir;			//0,����;1,����;
	uint16_t    width;			//LCD���
	uint16_t    height;			//LCD�߶�
	uint32_t    pixsize;		//ÿ��������ռ�ֽ���
    
}ltdc_dev; 

extern ltdc_dev lcdltdc;		            //����LCD LTDC����
extern LTDC_HandleTypeDef LTDC_Handler;	    //LTDC���
extern DMA2D_HandleTypeDef DMA2D_Handler;   //DMA2D���

#define LCD_PIXEL_FORMAT_ARGB8888       0X00    
#define LCD_PIXEL_FORMAT_RGB888         0X01    
#define LCD_PIXEL_FORMAT_RGB565         0X02       
#define LCD_PIXEL_FORMAT_ARGB1555       0X03      
#define LCD_PIXEL_FORMAT_ARGB4444       0X04     
#define LCD_PIXEL_FORMAT_L8             0X05     
#define LCD_PIXEL_FORMAT_AL44           0X06     
#define LCD_PIXEL_FORMAT_AL88           0X07      

///////////////////////////////////////////////////////////////////////
//�û��޸����ò���:
#define EXT_SDRAM_ADDR			0xC0000000		//ʹ��FMC_SDRAM_BANK1,Ҳ����BANK5����ַӳ�䵽 0xC0000000
									//FMC_SDRAM_BANK2,Ҳ����BANK6����ַӳ�䵽 0xD0000000
//������ɫ���ظ�ʽ,һ����RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
//����Ĭ�ϱ�������ɫ
#define LTDC_BACKLAYERCOLOR			0X00000000	
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define LCD_FRAME_BUF_ADDR			EXT_SDRAM_ADDR  

void LTDC_Switch(uint8_t sw);					//LTDC����
void LTDC_Layer_Switch(uint8_t layerx,uint8_t sw);	//�㿪��
void LTDC_Select_Layer(uint8_t layerx);			//��ѡ��
void LTDC_Display_Dir(uint8_t dir);				//��ʾ�������
void LTDC_Draw_Point(uint16_t x,uint16_t y,uint32_t color);//���㺯��
uint32_t LTDC_Read_Point(uint16_t x,uint16_t y);			//���㺯��
void LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);			//���ε�ɫ��亯��
void LTDC_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);	//���β�ɫ��亯��
void LTDC_Clear(uint32_t color);					//��������
uint8_t LTDC_Clk_Set(uint32_t pllsain,uint32_t pllsair,uint32_t pllsaidivr);//LTDCʱ������
void LTDC_Layer_Window_Config(uint8_t layerx,uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);//LTDC�㴰������
void LTDC_Layer_Parameter_Config(uint8_t layerx,uint32_t bufaddr,uint8_t pixformat,uint8_t alpha,uint8_t alpha0,uint8_t bfac1,uint8_t bfac2,uint32_t bkcolor);//LTDC������������
uint16_t LTDC_PanelID_Read(void);				//LCD ID��ȡ����
void LTDC_User_Init(void);						//LTDC��ʼ������

#endif 
