# -------------- #
# -- Makefile -- #
# -------------- #

# Copyright (c) 2000-2007 Lionel Lacassagne

# -- Lile list ----------
FILE = main.c nrutil.c mouvement.c mouvement_SSE.c morpho.c morpho_SSE.c morpho_THREAD.c morpho_SSE_THREAD.c ROC.c bench_mouvement.c bench_mouvement_SSE.c bench_morpho.c bench_morpho_SSE.c bench_morpho_THREAD.c bench_morpho_SSE_THREAD.c test_mouvement.c test_mouvement_SSE.c test_morpho.c test_morpho_SSE.c test_morpho_THREAD.c test_morpho_SSE_THREAD.c

# -- Paths ----------
SRC_PATH = src
OBJ_PATH = obj
EXE_PATH = exe
INC_PATH = include
MKDIR_P = mkdir -p
SD_FOLDER = SD SD+morpho
FD_FOLDER = FD FD+morpho

# -- OS ----------
#OS = MACH_OSX
OS = LINUX

# -- Config ----------
# if CONFIG = CLI  (Command Line Interface, no Apple Framework)
CONFIG = CLI

# -- Macros ----------
CC = gcc
AR = ar -rc

# -- Flags ----------
C_DEBUG_FLAGS = -O0
C_CC_FLAGS = -std=c99 -DNOALIAS -DALIGNED
C_OPTIMISATION_FLAGS = -O3 -fstrict-aliasing

#C_ARCH_FLAGS = -xSSE4.2
C_ARCH_FLAGS =

C_OS_FLAGS = -D$(OS)
C_CONFIG_FLAGS = -D$(CONFIG)
C_INC_FLAGS = -I$(INC_PATH)

CFLAGS =  $(C_CC_FLAGS) $(C_DEBUG_FLAGS)        $(C_ARCH_FLAGS) $(C_OS_FLAGS) $(C_CONFIG_FLAGS) $(C_INC_FLAGS) $(LIB_INC_PATH) 
CFLAGS = $(C_CC_FLAGS) $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_OS_FLAGS) $(C_CONFIG_FLAGS) $(C_INC_FLAGS) $(LIB_INC_PATH)
LDFLAGS = $(C_CC_FLAGS) $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_OS_FLAGS) $(C_CONFIG_FLAGS) $(C_INC_FLAGS) $(LIB_LIB_PATH)

# -- Final product ----------
PRODUCT   = projet.exe

# -- src and obj List ----------
SRC = $(addprefix ${SRC_PATH}/, $(FILE))
OBJ = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(FILE))))


all: directories $(EXE_PATH)/$(PRODUCT)

# -- Generate output folder ---------

directories:
	${MKDIR_P} ${EXE_PATH} ${OBJ_PATH} ${SD_FOLDER} ${FD_FOLDER}

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -fopenmp -Wall -std=gnu99
   
#-----Main rule ----------
$(EXE_PATH)/$(PRODUCT): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(OPTFLAGS) $(CFG) $(INC) $(LIB) -lm -lgomp

# -- Other stuff ----------
depend:
	makedepend $(CFLAGS) -Y $(SRC)

clean:
	rm -f $(OBJ)
	rm -f ${EXE_PATH}/${PRODUCT}
	rm -f SD/*.pgm
	rm -f FD/*.pgm
	rm -f FD+morpho/*.pgm
	rm -f SD+morpho/*.pgm
	rm -f visage/test_*.pgm

