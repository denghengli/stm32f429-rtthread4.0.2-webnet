#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "GUI.h"
#include "GUIDemo.h"


static void app_gui_thread(void *param)
{
    CRC_HandleTypeDef   CrcHandle;
    CrcHandle.Instance = CRC; 
    HAL_CRC_DeInit(&CrcHandle);
    HAL_CRC_Init(&CrcHandle);
    
    GUI_Init();
    GUI_Clear();
    WM_MULTIBUF_Enable(1);
    GUI_UC_SetEncodeUTF8();
    
    GUIDEMO_Main();
    
    while(1)
    {
        GUI_Delay(5); //ms
    }
}

static int app_gui(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("app_gui",
                           app_gui_thread, RT_NULL,
                           4096,
                           RT_THREAD_PRIORITY_MAX - 2, 10);
    
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    
    return RT_EOK;
}
//INIT_APP_EXPORT(gui_test_init);
