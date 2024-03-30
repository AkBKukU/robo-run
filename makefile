PATH = /bin:$(DEVKITARM)/bin:../../bin:/opt/devkitpro/tools/bin

PATH_LIB_GCC = $(DEVKITARM)/arm-none-eabi/lib/thumb

# name of application
NAME = "Robo Run"
TARGET = main

# Library
#LIBRARY:= $(wildcard lib/*.o)

# convert graphics
GRIT     = grit

# assemble and link
PREFIX  := arm-none-eabi-
GCC      = $(PREFIX)gcc
AS       = $(PREFIX)as
LD       = $(PREFIX)ld
OBJCOPY  = $(PREFIX)objcopy

# e-reader tools
NEVPK    = nevpk
NEDCMAKE = nedcmake
NEDCENC  = nedcenc
NEDCBMP  = raw2bmp
NESAV    = neflmake

SRC     := ./src
OBJ     := ./build
SRCS    := $(wildcard $(SRC)/*.c)
OBJS    := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))

# Supply "-DDEBUG_MGBA" here to enable mGBA printing
DEBUG ?=

all : sav bmp

sav : $(TARGET).sav

# E-Reader Dot Code Strip

bmp : build/$(TARGET).01.bin.raw
	$(eval RAW := $(shell ls build/$(TARGET).*.raw))
	$(foreach var,$(RAW),$(NEDCBMP) -i $(var) -o "$(var)";)
	mv build/*.bmp ./

build/$(TARGET).01.bin : build/$(TARGET).vpk
	$(NEDCMAKE) -i $< -o build/"$(TARGET)" -type 2 -bin -fill 1

build/$(TARGET).01.bin.raw : build/$(TARGET).01.bin 
	$(eval BIN := $(shell ls build/$(TARGET).*.bin))
	$(foreach var,$(BIN),$(NEDCENC) -i $(var) -o $(var).raw;)


# E-Reader Save Data
$(TARGET).sav : build/$(TARGET).vpk
	$(NESAV) -i $< -o "$@" -type 2 -name $(NAME)


# GBA Binary
build/$(TARGET).vpk : build/$(TARGET).bin
	$(NEVPK) -i "$<" -o "$@" -c -level 2

build/$(TARGET).bin : build/$(TARGET).elf
	$(OBJCOPY) -O binary "$<" "$@"

build/$(TARGET).elf : build/crt0.o build/map.o build/gfx_enemy.o build/gfx_player.o build/gfx_powerup.o build/map_boss.o $(OBJS)
	$(GCC) -lc -L $(PATH_LIB_GCC) -T ../../lib/ereader.ld -Os  $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c  gfx/gfx_player.c map/map.c map/map_boss.c
	$(GCC) $(DEBUG) -I ./ -I ../../lib  -I lib -Os -mthumb -c $< -o "$@"

build/gfx_enemy.o : gfx/gfx_enemy.c
	$(GCC) -I ../../lib  -I lib  -mthumb -c -O2 -o "$@" "$<"

build/gfx_player.o : gfx/gfx_player.c
	$(GCC) -I ../../lib  -I lib  -mthumb -c -O2 -o "$@" "$<"

build/gfx_powerup.o : gfx/gfx_powerup.c
	$(GCC) -I ../../lib  -I lib  -mthumb -c -O2 -o "$@" "$<"

build/map_boss.o : map/map_boss.c
	$(GCC) -I ../../lib  -I lib  -mthumb -c -O2 -o "$@" "$<"

build/map.o : map/map.c
	$(GCC) -I ../../lib  -I lib  -mthumb -c -O2 -o "$@" "$<"

build/crt0.o : ../../lib/crt0.s
	$(AS) -o "$@" "$<"


# Graphics
gfx/gfx_player.c:
	$(GRIT) gfx/player.tga -pS  -gu8 -pu16 -gB4 -ftc -fa -ogfx/gfx_player
#	$(GRIT) -pS -gu8 -pu8 -gB4 -ftc -fa -ogfx/gfx

gfx/gfx_enemy.c:
	$(GRIT) gfx/emy-*.tga gfx/eff-*.tga -pS  -gu8 -pu16 -gB4 -ftc -fa -ogfx/gfx_enemy

gfx/gfx_powerup.c:
	$(GRIT) gfx/bullet.tga gfx/powerup-*.tga -pS  -gu8 -pu8 -gB4 -ftc -fa -ogfx/gfx_powerup

map/map.c :
	$(GRIT) map/stars.tga map/tunnel.tga -gu8 -pu16 -mu16  -gB4 -ftc -pS -fa -m -omap/map

map/map_boss.c :
	$(GRIT) map/map_boss.tga -gu8 -pu16 -mu16  -gB4 -ftc -pS -fa -m -omap/map_boss

run : sav
	mGBA main.gba
clean :
	rm -f build/*.o
	rm -f build/*.bin
	rm -f build/*.elf
	rm -f build/*.raw
	rm -f build/*.vpk
	rm -f gfx/*.gfx gfx/*.pal gfx/*.c gfx/*.h
	rm -f map/*.gfx map/*.pal map/*.c map/*.h
	rm -f *.bmp *.sav
