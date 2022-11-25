#include "exti.h"

extern int Forward_or_reverse;		//正/反转状态 标志位
extern int Pause_or_continue;		//暂停/继续状态 标志位
extern int Compare1;				//比较寄存器的值

/*****************************************
引脚说明：

KEY0 连接PA0，选择下降沿触发
KEY1 连接PE2，选择下降沿触发
KEY2 连接PE3，选择下降沿触发
KEY3 连接PE4，选择下降沿触发
*****************************************/

void Exti_Init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOA clock */
	//使能（打开）GPIOA组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//使能（打开）GPIOE组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	/* Enable SYSCFG clock */
	//使能SYSCFG组时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;   	//输入模式
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;	//浮空模式
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;		//引脚0
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;		//引脚2
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3;		//引脚3
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;		//引脚4
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//PA0引脚--EXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	//PA2引脚--EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	//PA3引脚--EXTI3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	//PA3引脚--EXTI4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);

	/* Configure EXTI Line0 */
	//配置中断控制器
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //边沿触发 下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断线使能
	
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line0;			//中断线0	
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line2;			//中断线2	
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line3;			//中断线3
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line4;			//中断线4
	EXTI_Init(&EXTI_InitStructure);
	
	//设置NVIC分组 第二分组 抢占优先级范围:0~3  响应优先级范围：0~3
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	//NVIC配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;			//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 		//中断通道使能
	
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;  //中断通道设置 0
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI2_IRQn;  //中断通道设置 2
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI3_IRQn;  //中断通道设置 3
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI4_IRQn;  //中断通道设置 4
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************

a 中断服务函数是不能自定义的，只能是从startup_stm32f40_41xxx.s中查找
b 中断服务函数格式  void  中断服务函数(void)
c 中断服务函数是不需要进行调用的，当条件满足，CPU会自动进入中断服务函数执行
d 中断服务函数可以写在任意文件（自己写的.c文件）当中，但是只能写一次
e 中断服务函数是执行时间相对较短的程序

***************************************/
void EXTI0_IRQHandler(void)
{
	//SET == 1
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		delay_ms(10); //延时再判断按键KEY0是否按下   这里作用就是消抖
		//判断按键KEY0是否按下
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			//标志位判断，变更电机正反转状态
			if(Forward_or_reverse == 1) {
				
				//判断比较寄存器的值是否低于300
				//低于这个值无法正反转状态置换
				if (Compare1 <= 300) {
					TIM_SetCompare4(TIM1, 0);
					TIM_SetCompare3(TIM1, 999);
					TIM_SetCompare4(TIM4, 0);
					TIM_SetCompare3(TIM4, 999);
					delay_ms(15);	//设置启动值 再延时15毫秒
				}
				
				//设置比较寄存器4的值   参数值范围:0~999
				TIM_SetCompare4(TIM1, 0);		//设置为0 电机正转
				TIM_SetCompare4(TIM4, 0);		//设置为0 电机正转
				//设置比较寄存器3的值   参数值范围:0~999
				TIM_SetCompare3(TIM1, Compare1);		//设置为0 电机反转	反转需要按一下reset
				TIM_SetCompare3(TIM4, Compare1);		//设置为0 电机反转	反转需要按一下reset
				Forward_or_reverse = 0;
			} else {
				
				//判断比较寄存器的值是否低于300
				//低于这个值无法正反转状态置换
				if (Compare1 <= 300) {
					TIM_SetCompare3(TIM1, 0);
					TIM_SetCompare4(TIM1, 999);
					TIM_SetCompare3(TIM4, 0);
					TIM_SetCompare4(TIM4, 999);
					delay_ms(15);	//设置启动值 再延时15毫秒
				}
				//设置比较寄存器3的值   参数值范围:0~999
				TIM_SetCompare3(TIM1, 0);		//设置为0 电机反转	反转需要按一下reset
				TIM_SetCompare3(TIM4, 0);		//设置为0 电机反转	反转需要按一下reset
				//设置比较寄存器4的值   参数值范围:0~999
				TIM_SetCompare4(TIM1, Compare1);		//设置为0 电机正转
				TIM_SetCompare4(TIM4, Compare1);		//设置为0 电机正转
				Forward_or_reverse = 1;
			}
		}
		//清空中断标志0标志位
		EXTI_ClearITPendingBit(EXTI_Line0);	
	}
}

void EXTI2_IRQHandler(void)
{
	//SET == 1
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)
		{
			Compare1++;
			if (Compare1 >= 999)	//最高值
				Compare1 = 999;
		
			//变更电机正反转状态
			if(Forward_or_reverse == 0) {
				//设置比较寄存器3的值   参数值范围:0~999
				TIM_SetCompare3(TIM1, Compare1);
				TIM_SetCompare3(TIM4, Compare1);
			} else {
				//设置比较寄存器4的值   参数值范围:0~999
				TIM_SetCompare4(TIM1, Compare1);
				TIM_SetCompare4(TIM4, Compare1);
			}
			delay_ms(1);
		}
		
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 1)
		{
			//清空中断标志0标志位
			EXTI_ClearITPendingBit(EXTI_Line2);	
		}			
	}

}

void EXTI3_IRQHandler(void)
{
	//SET == 1
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
		{
			Compare1--;
			if (Compare1 <= 240)	//运行中最低的启动值
				Compare1 = 240;
			
			//变更电机正反转状态
			if(Forward_or_reverse == 0) {
				//设置比较寄存器3的值   参数值范围:0~999
				TIM_SetCompare3(TIM1, Compare1);
				TIM_SetCompare3(TIM4, Compare1);
			} else {
				//设置比较寄存器4的值   参数值范围:0~999
				TIM_SetCompare4(TIM1, Compare1);
				TIM_SetCompare4(TIM4, Compare1);
			}
			delay_ms(1);
		}
		
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 1)
		{
			//清空中断标志0标志位
			EXTI_ClearITPendingBit(EXTI_Line3);	
		}			
	}
}

void EXTI4_IRQHandler(void)
{
	//SET == 1
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		delay_ms(10); //延时再判断按键KEY3是否按下   这里作用就是消抖
		//判断按键KEY3是否按下
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
		{
			//变更电机暂停/继续状态
			if(Pause_or_continue == 0) {
				//变更电机正反转状态
				if(Forward_or_reverse == 0) {
					//设置比较寄存器3的值   参数值范围:0~999
					TIM_SetCompare3(TIM1, 0);
					TIM_SetCompare3(TIM4, 0);
				} else {
					//设置比较寄存器4的值   参数值范围:0~999
					TIM_SetCompare4(TIM1, 0);
					TIM_SetCompare4(TIM4, 0);
				}
				Pause_or_continue = 1;
			} else {
				//变更电机正反转状态
				if(Forward_or_reverse == 0) {
					if (Compare1 <= 300) {
						TIM_SetCompare3(TIM1, 999);
						TIM_SetCompare3(TIM4, 999);
						delay_ms(15);
					}
					//设置比较寄存器3的值   参数值范围:0~999
					TIM_SetCompare3(TIM1, Compare1);
					TIM_SetCompare3(TIM4, Compare1);
				} else {
					if (Compare1 <= 300) {
						TIM_SetCompare4(TIM1, 999);
						TIM_SetCompare4(TIM4, 999);
						delay_ms(15);
					}
					//设置比较寄存器4的值   参数值范围:0~999
					TIM_SetCompare4(TIM1, Compare1);
					TIM_SetCompare4(TIM4, Compare1);
				}
				Pause_or_continue = 0;
			}
		}
		//清空中断标志0标志位
		EXTI_ClearITPendingBit(EXTI_Line4);		
	}
}
