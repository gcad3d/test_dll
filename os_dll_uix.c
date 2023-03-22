/*  os_dll_uix.c
access  dll for Unix-OS
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                   // for ...
#include <ctype.h>                    // isdigit

#include <signal.h>
#include <errno.h>

#include <unistd.h>
#include <dlfcn.h>           // Unix: dlopen



#define FUNC_LOAD_only    20
#define FUNC_LOAD_all     21
#define FUNC_CONNECT      22
#define FUNC_EXEC         23
#define FUNC_UNLOAD       24
#define FUNC_RECOMPILE    25
#define FUNC_QUERY        26
#define FUNC_Init         27




//=====================================================================
  int OS_dll_do (char *dllNam, char *fncNam, void *fncDat, int mode) {
//=====================================================================
/// load dll dllNam; start function fncNam (fncDat); unload dll.
/// see also UI_DllLst_work
// Input:
//   fncnam  main-entry using datablock
//   fncdat  datablock
//   mode    0=load+start+unload
//           1=load+start               - do not unload (OS_dll_close_fn
//           2=unload


static void  *dl1=NULL;
static char  dlNamAct[256];
  char  *p1, s1[256];
  void  (*up1)();


  printf("OS_dll_do |%s|%s| %d\n",dllNam,fncNam,mode);
  // fflush (stdout);


  if(mode == 2) { mode = 0; goto L_close; }


  if(dl1 != NULL) {
    TX_Error("**** ERROR OS_dll_run: core-plugin |%s|  open ..",dllNam);
    return -1;
  }


  // fix DLL-FileName
  strcpy(dlNamAct, dllNam);
  // sprintf(s1, "%s%s.so",AP_get_bin_dir(),dllNam);
  sprintf(s1, "./%s.dll",dllNam);


  // load DLL
  dl1 = dlopen(s1, RTLD_LAZY);
  if(dl1 == NULL) {
    TX_Error("OS_dll_do: cannot open dyn. Lib. |%s|",dllNam);
    return -1;
  }


  // get adress of func. <fncNam>
  up1 = dlsym(dl1, fncNam);
  if(up1 == NULL) {
    OS_dll_close (&dl1);     // unload DLL
    TX_Error("OS_dll_do: cannot open Func. |%s|",fncNam);
    return -1;
  }
     printf(" OS_dll_do-dll_do-have-func\n");


  // start userprog
  (*up1)(fncDat);
     printf(" OS_dll_do-dll_do-after-func\n");


  // close DLL
  L_close:
  if(mode < 1) OS_dll_close (&dl1);     // unload DLL

    printf("ex-OS_dll_do\n");


  return 0;

}



//================================================================
  int OS_dll_close (void **dl1) {
//================================================================
// on successfule returns irc = 0,  dl1 = NULL
// BUG dlclose: returnCode is OK but handle not NULL.

  int  irc = 0;

  printf("OS_dll_close \n");

  // unload if already loaded
  if(*dl1 != NULL) {
    irc = dlclose (*dl1);  // 0=success
      // printf(" close %d\n",irc);
    if(!irc) *dl1 = NULL;
  }

  return irc;

}


// eof
