
TARGET			= noLED
SRCS			= main.c
OBJS			= main.o
BUILD_PRX		= 1
USE_KERNEL_LIBC	= 1
USE_KERNEL_LIBS = 1
PSP_FW_VERSION	= 661

PRX_EXPORTS		= exports.exp

LIBS			= -lpspsystemctrl_kernel  -lpsppower
LDFLAGS			= -nostartfiles

CFLAGS			= -Os -G0 -Wall -Wextra
CXXFLAGS		= $(CFLAGS)
ASFLAGS			= $(CFLAGS)

PSPSDK			= $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
