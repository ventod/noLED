#include <pspsyscon.h>
#include <psppower.h>
#include "systemctrl.h"

#define UNUSED __attribute__((unused))

#define SYSCON_CTRL_LED_SUCCES 0
#define AUTOASSIGNMENT -1
#define NO_ERROR 0

PSP_MODULE_INFO("noLED", 0x1000, 1, 0);
PSP_MAIN_THREAD_ATTR(0);

int sceSysconCtrlLED_nop(int SceLED UNUSED, int state UNUSED)
{
	return SYSCON_CTRL_LED_SUCCES;
}

void apply_patch(void)
{
	// get sceSysconCtrlLED address asap
	u32 address = sctrlHENFindFunction("sceSyscon_driver", "sceSysconCtrlLED", 0x18BFBE65);

	// turn off all leds
	sceSysconCtrlLED(0, LED_OFF); // MS
	sceSysconCtrlLED(1, LED_OFF); // WLAN
	sceSysconCtrlLED(2, LED_OFF); // POWER
	sceSysconCtrlLED(3, LED_OFF); // BT

	// redir sceSysconCtrlLED to sceSysconCtrlLED_nop
	if (address)
	{
		sctrlHENPatchSyscall((void *)address, sceSysconCtrlLED_nop);
	}
}

int resume_callback(int unknown UNUSED, int pwrflags, void *common UNUSED)
{
	if (pwrflags & PSP_POWER_CB_RESUMING)
	{
		apply_patch();
	}
	return NO_ERROR;
}

int module_start(SceSize args UNUSED, void *argp UNUSED)
{
	apply_patch();
	// create and register callback
	SceUID cbid = sceKernelCreateCallback("resume_callback_noLED", resume_callback, NULL);
	if (cbid)
	{
		scePowerRegisterCallback(-1, cbid);
	}
	return NO_ERROR;
}

int module_stop(SceSize args UNUSED, void *argp UNUSED)
{
	return NO_ERROR;
}
