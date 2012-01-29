# TOOL
CC = gcc
LINKER = gcc
RM = rm -rf
DEBUG = 1
SHELL = /bin/sh
Q = @

# COMMON CONFIG
CFLAGS := -Wall -mms-bitfields
INCLUDE_LIST := -I. -I/usr/local/include
LIBS :=
LIBS_LIST := -L/usr/local/lib 
DEFINE_LIST := 

# FFMPEG CONFIG
LIBS += -lswscale -lswresample -lavformat -lavcodec -lavutil -lavfilter -lavdevice -lm -lws2_32

# SDL CONFIG
INCLUDE_LIST += -I/usr/include/mingw -I/usr/local/include/SDL
CFLAGS += 
DEFINE_LIST += -D_GNU_SOURCE=1 -Dmain=SDL_main
#LIBS += `sdl-config --cflags --libs
#Removinw -mwindows to make console output
LIBS += -lmingw32 -lSDLmain -lSDL -liconv -lm -luser32 -lgdi32 -lwinmm #-mwindows 

# SOURCE and OBJECTS
OUTPUT := player.exe
SRCS :=
OBJS :=

OBJS += player_main/player_main.o 
OBJS += player_audio/player_audio.o


# DEBUG CHECK
ifeq ($(DEBUG), 1)
	DEFINE_LIST += -D__DEBUG__
endif


# FINAL
CFLAGS += $(DEFINE_LIST) $(INCLUDE_LIST)
vpath = %.c player_main player_audio util
vpath = %.o player_main player_audio util
vpath = %.d player_main player_audio util
DEPS = $(subst .o,.d,$(OBJS))

all: progs

progs: $(OBJS)
	@echo BUILD player.exe
	$(Q)$(CC) $(INCLUDE_LIST) $(LIBS_LIST) $(OBJS) -o $(OUTPUT) $(LIBS)	

clean:
	$(RM) $(OUTPUT) $(OBJS) $(DEPS)

%.d: %.c
	@echo DEP $<
	$(Q)set -e; rm -rf $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$; \
	sed 's,$(*F).o,$(@D)/$(*F).o, g' < $@.$$ > $@; \
	rm -rf $@.$$
	
%.o: %.c
	@echo CC  $<
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

include $(DEPS)

.PHONY: all clean 
