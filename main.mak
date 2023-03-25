# main.mak     build main[.exe]:
# make -f main.mak
# see README


ifeq "$(MAKE_HOST)" "x86_64-pc-linux-gnu"
# Linux
SRC = main.c os_dll_uix.c
else
# MSYS
SRC = main.c os_dll_ms.c
endif


#-----------------------------------------
# get <fn>.o from <fn>.c for sourceList
OBJ = $(patsubst %.c,%.o, $(notdir $(SRC)))

DEBUG = -ggdb -pg
CPFLG = $(DEBUG) -Wno-implicit-function-declaration

$(info - MAKE_HOST = $(MAKE_HOST))



#-----------------------------------------------------
ifeq "$(MAKE_HOST)" "x86_64-pc-linux-gnu"
# Linux-64:
LKFLG = $(DEBUG) -rdynamic -ldl -Wl,--unresolved-symbols=ignore-all

#-----------------------------------------------------
else
# MSYS:
LKFLG =  $(DEBUG) -Wl,-export-all-symbols -Wl,--out-implib,main.a
endif


#-----------------------------------------------------
build: $(OBJ)
	@echo ================= $@ ========================
	@echo "SRC=" $(SRC)
	@echo "OBJ=" $(OBJ)
	$(CC) $(OBJ) $(LKFLG) -o main


#-----------------------------------------------------
clean:
	rm -rf *.o *.obj *.out *.a *.ilk *.pdb *.exp *.lib *.dll *.so *.exe main


#-----------------------------------------------------
.c.o:
	@echo ================= Compile $@ ========================
	@echo "SRC=" $(SRC)
	@echo "OBJ=" $(OBJ)
	$(CC) -c $(CPFLG) $<


# eof
