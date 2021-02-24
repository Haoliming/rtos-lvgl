#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ILI93xx.h"
#include "touch.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_apps\demo\demo.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_1.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_2.h"
/************************************************
 ALIENTEK ս��STM32F103������ FreeRTOSʵ��4-1
 FreeRTOS�жϲ���-�⺯���汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//�������ȼ�
#define START_TASK_PRIO			1
//�����ջ��С	
#define START_STK_SIZE 			256  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define INTERRUPT_TASK_PRIO		2
//�����ջ��С	
#define INTERRUPT_STK_SIZE 		256  
//������
TaskHandle_t INTERRUPTTask_Handler;
//������
void interrupt_task(void *p_arg);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
	uart_init(115200);					//��ʼ������
//	LED_Init();		  					//��ʼ��LED
	
	TIM3_Int_Init(1000-1,72-1);		//��ʼ����ʱ��3����ʱ������1S
	TFTLCD_Init();		
	tp_dev.init();
	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
//	TIM5_Int_Init(10000-1,7200-1);		//��ʼ����ʱ��5����ʱ������1S
//	lv_test_theme_1();
	demo_create();
	while(1)
	{
		tp_dev.scan(0);
		lv_task_handler();
	}
	//������ʼ����
//    xTaskCreate((TaskFunction_t )start_task,            //������
//                (const char*    )"start_task",          //��������
//                (uint16_t       )START_STK_SIZE,        //�����ջ��С
//                (void*          )NULL,                  //���ݸ��������Ĳ���
//                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
//                (TaskHandle_t*  )&StartTask_Handler);   //������              
//    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //�����жϲ�������
    xTaskCreate((TaskFunction_t )interrupt_task,  			//������
                (const char*    )"interrupt_task", 			//��������
                (uint16_t       )INTERRUPT_STK_SIZE,		//�����ջ��С
                (void*          )NULL,						//���ݸ��������Ĳ���
                (UBaseType_t    )INTERRUPT_TASK_PRIO,		//�������ȼ�
                (TaskHandle_t*  )&INTERRUPTTask_Handler); 	//������
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//�жϲ��������� 
void interrupt_task(void *pvParameters)
{
	static u32 total_num=0;
    while(1)
    {
		total_num+=1;
		if(total_num==5) 
		{
			printf("�ر��ж�.............\r\n");
			portDISABLE_INTERRUPTS();				//�ر��ж�
			delay_xms(5000);						//��ʱ5s
			printf("���ж�.............\r\n");	//���ж�
			portENABLE_INTERRUPTS();
		}
        LED0=~LED0;
        vTaskDelay(1000);
    }
} 

