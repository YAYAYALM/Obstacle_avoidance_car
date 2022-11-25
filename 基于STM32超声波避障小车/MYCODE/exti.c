#include "exti.h"

extern int Forward_or_reverse;		//��/��ת״̬ ��־λ
extern int Pause_or_continue;		//��ͣ/����״̬ ��־λ
extern int Compare1;				//�ȽϼĴ�����ֵ

/*****************************************
����˵����

KEY0 ����PA0��ѡ���½��ش���
KEY1 ����PE2��ѡ���½��ش���
KEY2 ����PE3��ѡ���½��ش���
KEY3 ����PE4��ѡ���½��ش���
*****************************************/

void Exti_Init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOA clock */
	//ʹ�ܣ��򿪣�GPIOA��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//ʹ�ܣ��򿪣�GPIOE��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	/* Enable SYSCFG clock */
	//ʹ��SYSCFG��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;   	//����ģʽ
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;	//����ģʽ
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;		//����0
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;		//����2
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3;		//����3
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;		//����4
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//PA0����--EXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	//PA2����--EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	//PA3����--EXTI3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	//PA3����--EXTI4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);

	/* Configure EXTI Line0 */
	//�����жϿ�����
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //���ش��� �½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�ж���ʹ��
	
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line0;			//�ж���0	
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line2;			//�ж���2	
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line3;			//�ж���3
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line4;			//�ж���4
	EXTI_Init(&EXTI_InitStructure);
	
	//����NVIC���� �ڶ����� ��ռ���ȼ���Χ:0~3  ��Ӧ���ȼ���Χ��0~3
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;			//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 		//�ж�ͨ��ʹ��
	
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;  //�ж�ͨ������ 0
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI2_IRQn;  //�ж�ͨ������ 2
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI3_IRQn;  //�ж�ͨ������ 3
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI4_IRQn;  //�ж�ͨ������ 4
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************

a �жϷ������ǲ����Զ���ģ�ֻ���Ǵ�startup_stm32f40_41xxx.s�в���
b �жϷ�������ʽ  void  �жϷ�����(void)
c �жϷ������ǲ���Ҫ���е��õģ����������㣬CPU���Զ������жϷ�����ִ��
d �жϷ���������д�������ļ����Լ�д��.c�ļ������У�����ֻ��дһ��
e �жϷ�������ִ��ʱ����Խ϶̵ĳ���

***************************************/
void EXTI0_IRQHandler(void)
{
	//SET == 1
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		delay_ms(10); //��ʱ���жϰ���KEY0�Ƿ���   �������þ�������
		//�жϰ���KEY0�Ƿ���
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			//��־λ�жϣ�����������ת״̬
			if(Forward_or_reverse == 1) {
				
				//�жϱȽϼĴ�����ֵ�Ƿ����300
				//�������ֵ�޷�����ת״̬�û�
				if (Compare1 <= 300) {
					TIM_SetCompare4(TIM1, 0);
					TIM_SetCompare3(TIM1, 999);
					TIM_SetCompare4(TIM4, 0);
					TIM_SetCompare3(TIM4, 999);
					delay_ms(15);	//��������ֵ ����ʱ15����
				}
				
				//���ñȽϼĴ���4��ֵ   ����ֵ��Χ:0~999
				TIM_SetCompare4(TIM1, 0);		//����Ϊ0 �����ת
				TIM_SetCompare4(TIM4, 0);		//����Ϊ0 �����ת
				//���ñȽϼĴ���3��ֵ   ����ֵ��Χ:0~999
				TIM_SetCompare3(TIM1, Compare1);		//����Ϊ0 �����ת	��ת��Ҫ��һ��reset
				TIM_SetCompare3(TIM4, Compare1);		//����Ϊ0 �����ת	��ת��Ҫ��һ��reset
				Forward_or_reverse = 0;
			} else {
				
				//�жϱȽϼĴ�����ֵ�Ƿ����300
				//�������ֵ�޷�����ת״̬�û�
				if (Compare1 <= 300) {
					TIM_SetCompare3(TIM1, 0);
					TIM_SetCompare4(TIM1, 999);
					TIM_SetCompare3(TIM4, 0);
					TIM_SetCompare4(TIM4, 999);
					delay_ms(15);	//��������ֵ ����ʱ15����
				}
				//���ñȽϼĴ���3��ֵ   ����ֵ��Χ:0~999
				TIM_SetCompare3(TIM1, 0);		//����Ϊ0 �����ת	��ת��Ҫ��һ��reset
				TIM_SetCompare3(TIM4, 0);		//����Ϊ0 �����ת	��ת��Ҫ��һ��reset
				//���ñȽϼĴ���4��ֵ   ����ֵ��Χ:0~999
				TIM_SetCompare4(TIM1, Compare1);		//����Ϊ0 �����ת
				TIM_SetCompare4(TIM4, Compare1);		//����Ϊ0 �����ת
				Forward_or_reverse = 1;
			}
		}
		//����жϱ�־0��־λ
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
			if (Compare1 >= 999)	//���ֵ
				Compare1 = 999;
		
			//����������ת״̬
			if(Forward_or_reverse == 0) {
				//���ñȽϼĴ���3��ֵ   ����ֵ��Χ:0~999
				TIM_SetCompare3(TIM1, Compare1);
				TIM_SetCompare3(TIM4, Compare1);
			} else {
				//���ñȽϼĴ���4��ֵ   ����ֵ��Χ:0~999
				TIM_SetCompare4(TIM1, Compare1);
				TIM_SetCompare4(TIM4, Compare1);
			}
			delay_ms(1);
		}
		
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 1)
		{
			//����жϱ�־0��־λ
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
			if (Compare1 <= 240)	//��������͵�����ֵ
				Compare1 = 240;
			
			//����������ת״̬
			if(Forward_or_reverse == 0) {
				//���ñȽϼĴ���3��ֵ   ����ֵ��Χ:0~999
				TIM_SetCompare3(TIM1, Compare1);
				TIM_SetCompare3(TIM4, Compare1);
			} else {
				//���ñȽϼĴ���4��ֵ   ����ֵ��Χ:0~999
				TIM_SetCompare4(TIM1, Compare1);
				TIM_SetCompare4(TIM4, Compare1);
			}
			delay_ms(1);
		}
		
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 1)
		{
			//����жϱ�־0��־λ
			EXTI_ClearITPendingBit(EXTI_Line3);	
		}			
	}
}

void EXTI4_IRQHandler(void)
{
	//SET == 1
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		delay_ms(10); //��ʱ���жϰ���KEY3�Ƿ���   �������þ�������
		//�жϰ���KEY3�Ƿ���
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
		{
			//��������ͣ/����״̬
			if(Pause_or_continue == 0) {
				//����������ת״̬
				if(Forward_or_reverse == 0) {
					//���ñȽϼĴ���3��ֵ   ����ֵ��Χ:0~999
					TIM_SetCompare3(TIM1, 0);
					TIM_SetCompare3(TIM4, 0);
				} else {
					//���ñȽϼĴ���4��ֵ   ����ֵ��Χ:0~999
					TIM_SetCompare4(TIM1, 0);
					TIM_SetCompare4(TIM4, 0);
				}
				Pause_or_continue = 1;
			} else {
				//����������ת״̬
				if(Forward_or_reverse == 0) {
					if (Compare1 <= 300) {
						TIM_SetCompare3(TIM1, 999);
						TIM_SetCompare3(TIM4, 999);
						delay_ms(15);
					}
					//���ñȽϼĴ���3��ֵ   ����ֵ��Χ:0~999
					TIM_SetCompare3(TIM1, Compare1);
					TIM_SetCompare3(TIM4, Compare1);
				} else {
					if (Compare1 <= 300) {
						TIM_SetCompare4(TIM1, 999);
						TIM_SetCompare4(TIM4, 999);
						delay_ms(15);
					}
					//���ñȽϼĴ���4��ֵ   ����ֵ��Χ:0~999
					TIM_SetCompare4(TIM1, Compare1);
					TIM_SetCompare4(TIM4, Compare1);
				}
				Pause_or_continue = 0;
			}
		}
		//����жϱ�־0��־λ
		EXTI_ClearITPendingBit(EXTI_Line4);		
	}
}
