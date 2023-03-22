/* os_dll_ms.c
access od for MS-Win-OS
*/


#if defined _MSC_VER || __MINGW64__
#include <windows.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                   // for ...
#include <ctype.h>                    // isdigit

#include <signal.h>
#include <errno.h>

#ifndef _MSC_VER
#include <unistd.h>
#include <dlfcn.h>           // Unix: dlopen
#endif



#define FUNC_LOAD_only    20
#define FUNC_LOAD_all     21
#define FUNC_CONNECT      22
#define FUNC_EXEC         23
#define FUNC_UNLOAD       24
#define FUNC_RECOMPILE    25
#define FUNC_QUERY        26
#define FUNC_Init         27




//==================================================================
  int OS_dll_do (char* dllNam, char* fncNam, void* data, int mode) {
//==================================================================
// only MSVC and MSYS:
// MS-mode:   LoadLibrary ..
// Method MS-Win; unload not OK - FreeLibrary hangs

  int   irc;
  void  *dll1 = NULL;


  printf("OS_dll_do |%s|%s| %d\n",dllNam,fncNam,mode);

  if(mode == 2) { mode = 0; goto L_close; }


  // load and start dll
  irc =  OS_dll__ (&dll1, FUNC_LOAD_only, (void*)dllNam);
  if(irc < 0) return -1;

  // connect dll-function
  irc = OS_dll__ (&dll1, FUNC_CONNECT, (void*)fncNam);
  if(irc < 0) return -1;

  // call dll-function
  irc =  OS_dll__ (&dll1, FUNC_EXEC, NULL);
  if(irc < 0) return -1;

  // unload dll
  L_close:
  if(mode < 1) irc = OS_dll__ (&dll1, FUNC_UNLOAD, NULL);
  if(irc < 0) return -1;

    printf("ex-OS_dll_do\n");


  return irc;

}


//================================================================
  int OS_dll__ (void **dl1, int mode, void *fDat) {
//================================================================
/// load dll | start dll-function | unload dll
/// Input:
///   mode     FUNC_LOAD    = load DLL. fDat: dll-name without directory, fTyp.
///            FUNC_CONNECT = connect (connect Func fDat)
///            FUNC_EXEC    = work (call active Func with parameter fDat)
///            FUNC_UNLOAD  = unload active lib
///            FUNC_RECOMPILE = recompile DLL
/// Output:
///   dll      (address of) loaded DLL
///   retCod   0=OK; else error


  // typedef int (__stdcall *dllFuncTyp01)(void*);
  // static dllFuncTyp01 dll_up1;

//  static HINSTANCE    hdl1=NULL;
  static void*        (*dll_up1)();

  int   irc = 0;
  unsigned long  lrc = 123;
  char  s1[256], *p1;


  printf("OS_dll__ %d\n",mode);


  //----------------------------------------------------------------
  // mode 0 = open (load Lib fNam)
  if((mode == FUNC_LOAD_only)  ||
     (mode == FUNC_LOAD_all))      {

    // unload if already loaded
    if(*dl1 != NULL) {
      FreeLibrary (*dl1);
      *dl1 = NULL;
    }

    // load DLL
    // sprintf(s1, "%s%s.dll",AP_get_bin_dir(),(char*)fDat);
    sprintf(s1, "./%s.dll",(char*)fDat);
      printf(" dll=|%s|\n",s1); fflush(stdout);

    *dl1 = LoadLibrary (s1);
    if (*dl1 == NULL) {
      TX_Error("OS_dll__: cannot open dyn. Lib. |%s|",(char*)fDat);
      return -1;
    }
    dll_up1 = NULL;


  //----------------------------------------------------------------
  // mode 1 = connect (connect Func fDat)
  } else if(mode == FUNC_CONNECT) {

    // Adresse von Func.fncNam holen
    dll_up1 = (void*) GetProcAddress (*dl1, (char*)fDat);
    if(dll_up1 == NULL) {
      TX_Error("OS_dll__: cannot open Func. |%s|",(char*)fDat);
      FreeLibrary (*dl1);           // unload DLL
      *dl1 = NULL;
      return -1;
    }


  //----------------------------------------------------------------
  // mode 2 = work (call active Func with parameter fDat)
  } else if(mode == FUNC_EXEC) {

    // start userprog
    if(dll_up1 != NULL) {
      (*dll_up1) (fDat);

    } else  {
      TX_Error ("OS_dll__ E001");
      return -1;
    }


  //----------------------------------------------------------------
  // mode 3 = unload active lib
  } else if(mode == FUNC_UNLOAD) {

    // close DLL
    if(*dl1 != NULL) {
      FreeLibrary (*dl1);           // unload DLL
      // FreeLibraryAndExitThread (*dl1, lrc);           // unload DLL
      *dl1 = NULL;
    }


  //----------------------------------------------------------------
  // 4 = recompile dll
  } else if(mode == FUNC_RECOMPILE) {

    if(*dl1 != NULL) {
      FreeLibrary (*dl1);           // unload DLL
      *dl1 = NULL;
    }
    sprintf(s1, "%s.dll",(char*)fDat);
      printf(" dll=|%s|\n",s1); fflush(stdout);

    if(OS_dll_build (s1) != 0) {
       TX_Error("OS_dll__: compile/link %s",s1);
       return -1;
    }


  //----------------------------------------------------------------
  }


    printf("ex-OS_dll__ mode=%d irc=%d\n",mode,irc);

  return 0;

}


// eof
