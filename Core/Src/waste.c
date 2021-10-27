/**
 ******************************************************************************
 * @file    waste.c
 * @brief   This file provides code for the configuration and utils of
 *          the waste
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 cloud.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

/*
 * 							  	   WASTE STATE MACHINE
 * 								  ---------------------
 *
 *
 *				  ____	 	       _____ 	 		 ___	 	  ____
 *				 |	  |		      |	    |		    |   |		 |    |
 *				 |	  v		      |	    v		    |   v		 |	  v
 *				---------		-----------		  ---------	    ---------
 *				|		|		|		  |		  |		  |     |		|
 *				| IDLE  | ----> | MEASURE | ----> |	SEND  |---->| SLEEP	|
 *				|		|		|  		  |		  |		  |		|		|
 *				---------		-----------		  ---------		---------
 *					^						                    	|
 *					|												|
 *					|_______________________________________________|
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "waste.h"
#include "usr.h"
#include "tim.h"
#include "rtc.h"
#include "lte.h"
#include "usart.h"
#include "adc.h"
#include <stdio.h>

waste_typedef wasteParameters;
debugMessages_typedef debugMessages;
void WasteInit() {
	HAL_TIM_Base_Start(&htim2);
	usrParameters.measureRepCount = USR_MEASURE_COUNT;
	usrParameters.measurementValue = 0;
	lteParameters.contexId = 1;
	lteParameters.sessionId = 0;
	LTEInitParams();
	for (int i = 0; i < 15; ++i) {
		lteParameters.imeiNum[i] = '*';
		lteParameters.imsiNum[i] = '*';
	}

	rtcParameters.wakeUpFlag = 1;
	wasteParameters.wakeUpCount = 0;
	wasteParameters.setupCount=0;
wasteParameters.setupMode=1;
	adcParameters.adcReadEnableFlag = 1;
	lteParameters.powerState = PWR_PASSIVE;

}

void DebugSendData(const char *msg, uint8_t msgSize) {
#ifdef DEBUG
	memset(debugMessages.MEESAGE, '-', 138);
	for (int i = 0; i < msgSize; ++i) {
		if (i < 138) {
			debugMessages.MEESAGE[i] =(char) msg[i];
		}
	}
	debugMessages.MEESAGE[136] = '\r';
	debugMessages.MEESAGE[137] = '\n';
	HAL_UART_Transmit(&huart2, (uint8_t*) debugMessages.MEESAGE, sizeof(debugMessages.MEESAGE), HAL_MAX_DELAY); // Send message to the debug
#endif

}

/************************ (C) COPYRIGHT cloud *****END OF FILE****/