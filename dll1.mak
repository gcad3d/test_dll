# build dll1.dll/dll1.o:
# make -f dll1.mak
# build lib_main_exp.a:
# dlltool -v -l lib_main_exp.a -d main_exp.def


SRC = dll1.c

#-----------------------------------------------------
# get <fn>.o from <fn>.c for sourceList
OBJ = $(patsubst %.c,%.o, $(notdir $(SRC)))

DEBUG = -g3 -O0

CPFLG =  -fPIC $(DEBUG) -Wno-implicit-function-declaration
#CPFLG =  -MMD $(DEBUG) -Wno-implicit-function-declaration
#CPFLG =  $(DEBUG) -MMD -Wno-implicit-function-declaration

$(info - MAKE_HOST = $(MAKE_HOST))

#-----------------------------------------------------
ifeq "$(MAKE_HOST)" "x86_64-pc-linux-gnu"
LKFLG = $(DEBUG) -shared -export-dynamic

#-----------------------------------------------------
else
# MSYS: x86_64-pc-msys
LKFLG = -shared $(DEBUG) main.a

endif

#-----------------------------------------------------
build: $(OBJ)
	@echo ================= $@ ========================
	@echo "SRC=" $(SRC)
	@echo "OBJ=" $(OBJ)
	$(CC) $(OBJ) $(LKFLG) -o dll1.dll


#-----------------------------------------------------
.c.o:
	@echo ================= Compile $@ ========================
	$(CC) -c $(CPFLG) $<


# eof
