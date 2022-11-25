//#include "get_distance.h"
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "car_ctrl.h"
#include "car_auto.h"
#include "string.h"
#include "usart1.h"
#include "stdio.h"
#include "delay.h"
#include "l298n.h"
#include "srf05.h"
#include "task.h"
#include "hc05.h"
#include "sg90.h"

INPUT	bluetooth_data;

///* 任务2 -- 蓝牙控制小车 */
//static TaskHandle_t car_ctrl_handle = NULL;
//static void car_ctrl(void* pvParameters);

int main(void)
{
	Sg90_Init();
	Srf05_Init();
	Delay_Init();
	Usart1_Init(9600);
	L298n_Ctrl1_Init();
	L298n_Ctrl2_Init();
	Hc05_Usart3_Init(9600);

//	/* 创建蓝牙控制小车任务 */
//	xTaskCreate((TaskFunction_t )car_ctrl,  		/* 任务入口函数 */
//			  (const char*    )"car_ctrl",			/* 任务名字 */
//			  (uint16_t       )512,  				/* 任务栈大小 */
//			  (void*          )NULL,				/* 任务入口函数参数 */
//			  (UBaseType_t    )6, 					/* 任务的优先级 */
//			  (TaskHandle_t*  )&car_ctrl_handle);	/* 任务控制块指针 */

//		/* 开启任务调度 */
//	vTaskStartScheduler();
				
	while(1)
	{
		if (GetData_Done == 1) {
			
			bluetooth_data = DATARecv();
		
			if (bluetooth_data.mode == 1) {
				Car_Auto();
			} else {
				Car_Ctrl();
			}
		}
	}
}

//static void car_ctrl(void* pvParameters)
//{
//	for(;;)
//	{
//		Car_Ctrl();
//	}
//}

/*-----------------------------------------------------------*/
//pvPortMalloc申请失败会调用此钩子函数 内存分配失败
void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/
//空闲任务钩子函数
void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
	
	__disable_irq();
	
	//等待指令和数据同步
	__dsb(portSY_FULL_READ_WRITE );
	__isb(portSY_FULL_READ_WRITE );
	
	__enable_irq();
	

	__wfi();				//进入睡眠模式

	
	//等待指令和数据同步
	__disable_irq();
	__dsb(portSY_FULL_READ_WRITE );
	__isb(portSY_FULL_READ_WRITE );
	__enable_irq();	
	
}
/*-----------------------------------------------------------*/
//栈溢出钩子函数
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

//static u32 gflag = 0;

//心跳钩子函数每次执行 每1ms会执行
void vApplicationTickHook( void )
{
//	gflag++;
//	if(gflag == 1000)
//	{
//		gflag = 0;
//		GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
//	}
}
