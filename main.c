#include <pspmoduleinfo.h>
#include <pspsyscon.h>

#define UNUSED __attribute__((unused))

#define NO_ERROR 0

PSP_MODULE_INFO("noLED", 0x1000, 1, 0);
PSP_MAIN_THREAD_ATTR(0);

int module_start(SceSize args UNUSED, void *argp UNUSED)
{
	// power off to all leds
	sceSysconCtrlLED(0, LED_OFF); // MS
	sceSysconCtrlLED(1, LED_OFF); // WLAN
	sceSysconCtrlLED(2, LED_OFF); // POWER
	sceSysconCtrlLED(3, LED_OFF); // BT
	return NO_ERROR;
}

int module_stop(SceSize args UNUSED, void *argp UNUSED)
{
	return NO_ERROR;
}
