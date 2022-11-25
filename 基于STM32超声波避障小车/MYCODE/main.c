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

///* ����2 -- ��������С�� */
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

//	/* ������������С������ */
//	xTaskCreate((TaskFunction_t )car_ctrl,  		/* ������ں��� */
//			  (const char*    )"car_ctrl",			/* �������� */
//			  (uint16_t       )512,  				/* ����ջ��С */
//			  (void*          )NULL,				/* ������ں������� */
//			  (UBaseType_t    )6, 					/* ��������ȼ� */
//			  (TaskHandle_t*  )&car_ctrl_handle);	/* ������ƿ�ָ�� */

//		/* ����������� */
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
//pvPortMalloc����ʧ�ܻ���ô˹��Ӻ��� �ڴ����ʧ��
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
//���������Ӻ���
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
	
	//�ȴ�ָ�������ͬ��
	__dsb(portSY_FULL_READ_WRITE );
	__isb(portSY_FULL_READ_WRITE );
	
	__enable_irq();
	

	__wfi();				//����˯��ģʽ

	
	//�ȴ�ָ�������ͬ��
	__disable_irq();
	__dsb(portSY_FULL_READ_WRITE );
	__isb(portSY_FULL_READ_WRITE );
	__enable_irq();	
	
}
/*-----------------------------------------------------------*/
//ջ������Ӻ���
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

//�������Ӻ���ÿ��ִ�� ÿ1ms��ִ��
void vApplicationTickHook( void )
{
//	gflag++;
//	if(gflag == 1000)
//	{
//		gflag = 0;
//		GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
//	}
}
