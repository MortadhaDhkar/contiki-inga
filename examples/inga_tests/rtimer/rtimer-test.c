/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

#include "contiki.h"

#include <stdio.h> /* For printf() */
#include "rtimer.h"
/*---------------------------------------------------------------------------*/
PROCESS(rtimer_test_process, "RTimer Test process");
AUTOSTART_PROCESSES(&rtimer_test_process);
/*---------------------------------------------------------------------------*/
int RUNNING = 1;
struct etimer et;
struct rtimer rt;
uint16_t cur_time, after_rtimer;
int ret;	
uint8_t DONE;
void rtime_call(struct rtimer *t, void *ptr){
	after_rtimer=RTIMER_NOW();
	if (after_rtimer-cur_time ==  1000+1){
		ret=0;
	}else{
		ret=1;
	}
	DONE=2;
}
PROCESS_THREAD(rtimer_test_process, ev, data)
{
	PROCESS_BEGIN();

	etimer_set(&et, CLOCK_SECOND * 5);
	PROCESS_YIELD_UNTIL(etimer_expired(&et));

	while(RUNNING) {
		if(DONE==0){
			printf("Starting test...\n");
				cur_time=RTIMER_NOW();
				rtimer_set(&rt, cur_time + 1000, 0, rtime_call,NULL);
		}
		if(DONE==2){
			printf("Test finished with %d\n", ret);

			printf("Result: ");
			if( ret == 0 ) {
				printf("SUCCESS\n");
				RUNNING = 0;
			} else {
				printf("FAILURE timediff = %d\n",after_rtimer-cur_time);
				RUNNING = 0;

			}
		}
		etimer_set(&et, CLOCK_SECOND * 1);
		PROCESS_YIELD_UNTIL(etimer_expired(&et));
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/