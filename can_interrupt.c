/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J?lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"
//#include "motor_control.h"

#include <stdio.h>
#include "sam.h"

//#include "../uart_and_printf/printf-stdarg.h"
#include "printf-stdarg.h"

#include "can_controller.h"

#define DEBUG_INTERRUPT 0

PID_DATA* my_pid;
PID_DATA hey;
int PID_INIT_FLAG =1;
/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */

/*void pid_init_can(){
	my_pid=pid_init(15, 3, 50);
}*/

void CAN0_Handler( void )
{
	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);
			printf("x,y = %d,%d - BTN_L, BTN_R = %d,%d - slider_L, slider_R = %d, %d\r", (int8_t)message.data[0], (int8_t)message.data[1], message.data[2], message.data[3], message.data[4], message.data[5]);
			//printf("Button = %d\r", message.data[0]);
			
			if (PID_INIT_FLAG && message.id==10)
			{
				hey=pid_init(15, 3, 10);
				PID_INIT_FLAG=0;
				printf("cool, not cool");
				calibrate_encoder();
			}
			else if (PID_INIT_FLAG && message.id==11)
			{
				hey=pid_init(15, 3, 20);
				PID_INIT_FLAG=0;
				printf("cool, not cool");
				calibrate_encoder();
			}
			else if (PID_INIT_FLAG && message.id==12)
			{
				hey=pid_init(15, 3, 25);
				PID_INIT_FLAG=0;
				printf("cool, not cool");
				calibrate_encoder();
			}
			
			if (message.data[3]) PIOC->PIO_CODR = PIO_PC9; // turn solenoid on and pin 41 low
			else PIOC->PIO_SODR = PIO_PC9;
			//PIOC->PIO_ABSR |= PIO_PER_P4;
// 			PMC->PMC_PCER0 |= PMC_PCER0_PID11; // PIOA power ON
// 			PIOA->PIO_PER =PIO_PA15;
// 			PIOA->PIO_OER = PIO_PA15;
// 			PIOA->PIO_ODR = PIO_PA15;
			
			printf("PID: %d %d", my_pid->i, my_pid->integral_sum_error);
			int8_t x_joy = message.data[0];
			int32_t ref = (x_joy + 128) * 16000/256 + 1000;
			set_motor(pid_controller(ref, feedback_encoder(), &hey ));
			
			pwm_set_relative_duty(2*(50-message.data[5]));
			if (message.data[2])
			{
				PID_INIT_FLAG = 1;
			}
			
		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		{
			can_receive(&message, 2);
// 			printf("Slider val = %d\r", message.data[0]);
// 			pwm_set_relative_duty(message.data[0]);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}
