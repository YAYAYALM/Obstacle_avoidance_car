# Obstacle_avoidance_car
Based on STM32 ultrasonic obstacle avoidance car
接线说明

蓝牙串口通信模块
        HC_05：

            STATE  -------  X      蓝牙连接状态指示       空脚
            RXD    -------  PB10   蓝牙输入端：          引脚复用：USART3_TX
            TXD    -------  PB11   蓝牙输出端：          引脚复用：USART3_RX
            GND    -------  GND    蓝牙接地端：          板载接地
            VCC    -------  VCC    蓝牙供电端：          板载供电 +5V
            EN     -------  X      蓝牙使能端：          空脚

电机驱动板模块
        L298N：

            IN1	-------	PE13	电机_左轮：	引脚复用：PWM	TIM1_CH3
            IN2	-------	PE14	电机_左轮：	引脚复用：PWM	TIM1_CH4
            IN3	-------	PD14	电机_右轮：	引脚复用：PWM	TIM4_CH3
            IN4	-------	PD15	电机_右轮：	引脚复用：PWM	TIM4_CH4

舵机驱动模块
        SG90：

            VCC     -------  VCC	舵机供电端：      板载供电 +5V
            PWM_IN  -------  PE5	舵机脉冲输入端：   引脚复用：PWM	TIM9_CH1
            GND     -------  GND	舵机供电端：      板载供电 +5V

超声波测距模块
        HY-SRF05：

            VCC   -------  VCC   超声波供电端：        板载供电 +5V
            TRIG  -------  PA2   超声波触发控制端：     普通输出：GPIOA_Pin_2
            ECHO  -------  PA3   超声波回响信号端：     普通输出：GPIOA_Pin_3
            OUT   -------  X     超声波开关量输出端：   空脚
            GND   -------  GND   超声波供电端：        板载接地
	    
蓝牙串口通讯：
    数据包协议：
            包头           原数据          效验和           包尾
            1字节          自定义          1字节           1字节
            固定值          自定义	      数据所有         固定值
            0x5A                         字节之和         0x5A
                                         的低8位
