
#include <rtt.h>
#include <board.h>
#include <dbgu.h>
#include <stdio.h>

#define STD_ALARM_VAL 0x1	//1 ms standard value
#define STD_PRESET 0x0020 //1 ms preset


static unsigned int flag;


typedef struct
{
	void* device;
	unsigned int device_id;
	unsigned int alarm_value;
	unsigned short rtpres;
} rtt_device;


static rtt_device rtt ={
	AT91C_BASE_RTTC,
	AT91C_ID_SYS,
	STD_ALARM_VAL,
	STD_PRESET
};


void rtt_int(void)
{
	//disable interrupt
	AT91F_AIC_DisableIt(AT91C_BASE_AIC, rtt.device_id);
	flag = 1;
}

void rtwait(unsigned int ms)
{
	flag = 0;
	//clear interrupt fields
	AT91F_RTTGetStatus(rtt.device);
	
	//setting relevant registers in the rtt device
	AT91F_RTTSetPrescaler(rtt.device, rtt.rtpres);
	AT91F_RTTSetAlarmValue(rtt.device, rtt.alarm_value*ms);
	AT91F_RTTSetAlarmINT(rtt.device);
	AT91F_RTTClearRttIncINT(rtt.device);
	
	//registering the interrupt (normally in rtt init!)
	AT91F_AIC_ConfigureIt(AT91C_BASE_AIC, rtt.device_id, 
							AT91C_AIC_PRIOR_LOWEST, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, 
							(void*)rtt_int);
	AT91F_AIC_EnableIt(AT91C_BASE_AIC, rtt.device_id);
	AT91F_RTTRestart(rtt.device);
	while(!flag);
}
