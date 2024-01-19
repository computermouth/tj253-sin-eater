
IFLAGS ?=
LFLAGS ?=
EFLAGS ?=

PLATFORM ?= PLATFORM_DESKTOP

RAYLIB_SRC ?= ext/raylib/src
RAYLIB_LIB ?= libraylib.a
RAYLIB_PLT ?= libraylib_$(PLATFORM).a

IFLAGS += -I$(RAYLIB_SRC)
LFLAGS += -L$(RAYLIB_SRC) -lraylib_$(PLATFORM) -lm

BIN ?= crayjam

## platform-specifics
ifeq ($(PLATFORM),PLATFORM_WEB)
	BIN = index.html
	EFLAGS += -s ASYNCIFY
	EFLAGS += -s USE_GLFW=3
	EFLAGS += -s TOTAL_MEMORY=128MB
	EFLAGS += -s FORCE_FILESYSTEM=1
	EFLAGS += -DPLATFORM_WEB
	EFLAGS += --shell-file data/minshell.html
	EFLAGS += --preload-file res
	CC = emcc
else ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(OS),Windows_NT)
		PLATFORM_OS = WINDOWS
		LFLAGS += -lopengl32 -lgdi32 -lwinmm -lcomdlg32 -lole32
	else
		UNAMEOS = $(shell uname)
		ifeq ($(UNAMEOS),Linux)
			PLATFORM_OS = LINUX
			LFLAGS += -lGL -lrt -ldl -lm -lX11 -lpthread
		endif
		ifeq ($(UNAMEOS),Darwin)
			PLATFORM_OS = OSX
			LFLAGS += -framework Foundation
			LFLAGS += -framework Cocoa
			LFLAGS += -framework OpenGL
			LFLAGS += -framework CoreAudio
			LFLAGS += -framework CoreVideo
			LFLAGS += -framework IOKit
		endif
	endif
endif

BUILD_DEP = $(wildcard src/*) Makefile $(wildcard res/*)
C_SRC = $(wildcard src/*.c)

out/$(BIN): $(RAYLIB_SRC)/$(RAYLIB_PLT) $(BUILD_DEP)
	$(CC) -o out/$(BIN) $(C_SRC) $(IFLAGS) $(LFLAGS) $(EFLAGS)

$(RAYLIB_SRC)/$(RAYLIB_PLT):
	$(MAKE) -C $(RAYLIB_SRC) clean
	$(MAKE) -C $(RAYLIB_SRC) PLATFORM=$(PLATFORM)
	mv $(RAYLIB_SRC)/$(RAYLIB_LIB) $(RAYLIB_SRC)/$(RAYLIB_PLT)