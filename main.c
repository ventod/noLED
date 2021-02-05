#include <pspsyscon.h>
#include <psppower.h>
#include "systemctrl.h"

#define UNUSED __attribute__((unused))

#define SCE_LED_SET_MODE_NID 0xEA24BE03

#define NO_ERROR 0

#define MAKE_DUMMY_FUNCTION_RETURN_0(a) \
	do                                  \
	{                                   \
		_sw(0x03E00008, a);             \
		_sw(0x00001021, a + 4);         \
	} while (0) // from utils.h procfw

#define SCE_ERROR_OK 0					     // from errors.h uOFW
#define POWER_CALLBACK_TOTAL_SLOTS_KERNEL 32 // from power.h uOFW
#define SCE_LED_MODE_OFF 0				     // from led.h uOFW

typedef struct
{
	u8 selectiveCmd;
	u8 numExecs;
	u8 customCmd;
	u8 unk;
	s32 onTime;
	s32 offTime;
	s32 blinkTime;
	s32 endBlinkState;
} SceLedConfiguration; // from led.h uOFW

PSP_MODULE_INFO("noLED", 0x1000, 1, 0);
PSP_MAIN_THREAD_ATTR(0);

int noLedPower(int unknown UNUSED, int pwrflags, void *common UNUSED)
{
	if (pwrflags & PSP_POWER_CB_RESUME_COMPLETE)
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
	// get sceLedSetMode address
	u32 address = sctrlHENFindFunction("sceLED_Service", "sceLed_driver", SCE_LED_SET_MODE_NID);
	if (address)
	{
		// as function
		s32 (*sceLedSetMode)(s32, s32, SceLedConfiguration *) = (void *)address;
		// turn off leds
		sceLedSetMode(0, SCE_LED_MODE_OFF, NULL); // MS
		sceLedSetMode(1, SCE_LED_MODE_OFF, NULL); // WLAN
		sceLedSetMode(3, SCE_LED_MODE_OFF, NULL); // BT
		// patch sceLedSetMode
		MAKE_DUMMY_FUNCTION_RETURN_0(address);
		// clear caches
		sceKernelDcacheWritebackAll();
		sceKernelIcacheClearAll();
	}
	// also remove power from leds
	noLedPower(0, PSP_POWER_CB_RESUME_COMPLETE, NULL);
	// create and register callback to
	SceUID cbid = sceKernelCreateCallback("noLedPower", noLedPower, NULL);
	if (cbid)
	{
		s32 status = !SCE_ERROR_OK;
		for (s32 slot = 0; slot < POWER_CALLBACK_TOTAL_SLOTS_KERNEL && status != SCE_ERROR_OK; ++slot)
		{
			status = scePowerRegisterCallback(slot, cbid);
		}
	}

	return NO_ERROR;
}

int module_stop(SceSize args UNUSED, void *argp UNUSED)
{
	return NO_ERROR;
}
