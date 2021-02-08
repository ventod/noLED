#include <pspsyscon.h>
#include <pspsysevent.h>
#include <pspmoduleinfo.h>

#define UNUSED __attribute__((unused))

#define NO_ERROR 0

#define SCE_RESUME_EVENTS 0x00FF0000 
#define SCE_SYSTEN_RESUME_EVENT_COMPLETED 0x00400000

PSP_MODULE_INFO("noLED", 0x1000, 1, 0);
PSP_MAIN_THREAD_ATTR(0);

int noLedPower(int ev_id, char *ev_name UNUSED, void *param UNUSED, int *result UNUSED)
{
	if (ev_id & SCE_SYSTEN_RESUME_EVENT_COMPLETED)
	{
		// power off to all leds
		sceSysconCtrlLED(0, LED_OFF); // MS
		sceSysconCtrlLED(1, LED_OFF); // WLAN
		sceSysconCtrlLED(2, LED_OFF); // POWER
		sceSysconCtrlLED(3, LED_OFF); // BT
	}
	return NO_ERROR;
}

int module_start(SceSize args UNUSED, void *argp UNUSED)
{
	// remove power from leds
	noLedPower(SCE_SYSTEN_RESUME_EVENT_COMPLETED, NULL, NULL, NULL);
	// create handler
	PspSysEventHandler handler;
	handler.size = 0x40;
	handler.name = "";
	handler.type_mask = SCE_RESUME_EVENTS;
	handler.handler = noLedPower;
	// register handler
	sceKernelRegisterSysEventHandler(&handler);
	return NO_ERROR;
}

int module_stop(SceSize args UNUSED, void *argp UNUSED)
{
	return NO_ERROR;
}
