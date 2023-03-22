/* main.c
mainmodule  - exports main_export and i1TestMain -> dll1
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




int i1TestMain;                       // export to dll
int main_export (char *txt);          // export to dll



void TX_Error (char* txt, ...);



//================================================================
  int main (int argc, char *argv[]) {
//================================================================

  int          irc;
  char         s1[64];
  static char  *dllNam = "dll1";
  static char  *fncNam = "dll_main_start";



  printf("\n=========================\n");
  printf(" test_dll_main starting ..\n");

  i1TestMain = 123;

  main_export ("-- in main 1 --");


  //----------------------------------------------------------------
  L_start:

  // MSYS and MSVC; LoadLibrary ..
  // 0=load+start+unload; 1=load+start; 2=unload;
  irc = OS_dll_do (dllNam, fncNam, NULL, 0);
  if(irc < 0) goto L_exit;

  main_export ("-- in main 2 --");

  goto L_reComp;




  //----------------------------------------------------------------
  L_reComp:

  // get key from user; Esc to exit, CR to edit dll1.c
  printf("- key e CR for edit dll1.c\n");
  printf("- key CR for exit\n");
// TODO: make func OS_keyb_get (char* sIn, int sSiz)


  s1[0] = getchar ();
  if(s1[0] == 10) goto L_exit;
  s1[0] = getchar ();  // get char 10



  //----------------------------------------------------------------
  // reuse dll ..
  // check in dll-output ..
  i1TestMain = 456;

  main_export ("-- in main 20 --");
  goto L_start;




  //----------------------------------------------------------------
  L_exit:
    main_export (" exit test_dll_main");
    printf("=========================\n\n");
  return 0;

}


//================================================================
  int main_export (char *txt) {
//================================================================
// func to be used by dll

  printf(" main_export |%s|\n",txt);

  return 0;

}


//===================================================================
  int OS_dll_build (char *dllNam) {
//===================================================================
// recompile dll


  int  irc;
  char *sdir, cbuf[256];         // char cbuf[512];


  printf("OS_dll_build |%s|\n",dllNam);


//   // sprintf(cbuf, "%sxa\\%s",AP_get_bas_dir(),dllNam);
//   // sprintf(cbuf, "%s..\\src\\APP\\%s",AP_get_loc_dir(),dllNam);
//   sprintf(cbuf, "%ssrc\\APP\\%s", sdir, dllNam);
// 
// 
//   // ".dll" -> ".nmak"
//   strcpy(&cbuf[strlen(cbuf)-4], ".nmak");
//     // printf(" exist: |%s|\n",cbuf);
//   if(OS_checkFilExist (cbuf, 1) == 0) goto L_err_nof;
// 
// 
//   printf(".. compile .. link .. %s",dllNam);
// 
//   // sprintf(cbuf, "cd %sxa&&nmake -f %s",AP_get_bas_dir(),dllNam);
//   // sprintf(cbuf, "cd %s..\\src\\APP&&nmake -f %s",AP_get_loc_dir(),dllNam);
//   sprintf(cbuf, "CMD /C \"make -f %s\"", sdir, dllNam);
//     // printf(" OS_dll_build 2 |%s|\n",cbuf);
// 
//   irc = OS_system(cbuf);
//   if(irc != 0) TX_Error("Error build %s",dllNam);
//
//  return irc;
//
//  L_err_nof:
//    printf("***** %s does not exist ..",cbuf);

    return 0;

}



//====================================================================
  int OS_debug_dll_ (char *dllNam) {
//====================================================================
// wenn im Debug-Modus wird hier gestoppt;
// In .gdbinit  muss stehen:
// break OS_debug_dll_
// wenn Debugger steht, eingeben "watch varnam" und "c"
//
// Zweck: Debugger kann DLL nur ansprechen wenn geladen.

  printf("OS_debug_dll_ |%s|\n",dllNam);

  return 0;

}


#if defined _MSC_VER || __MINGW64__
// Windows - MS-VC or MSYS

//================================================================
  int OS_checkFilExist (char* filnam, int mode) {
//================================================================
// OS_checkFilExist         check if File or Directory exists
// mode = 0: display message sofort;
// mode = 1: just fix returncode, no message
//
// rc = 0: no, file does NOT exist, error
// rc = 1: yes, file exists, OK.


  int     i1;
  char    fn[512];

  printf("OS_checkFilExist |%s| %d\n", filnam, mode);

//   if(strlen(filnam) > 510) {
//     TX_Error ("OS_checkFilExist E1"); exit (-1);
//   }
//
//   sprintf(fn, "\"%s\a",filnam);

  i1 = GetFileAttributes (filnam);
    // printf(" GetFileAttributes %d %s\n",i1,filnam);
  // returns -1=file not found; 16=file, 32=directory ?
    // printf("GetFileAttributes %d |%s|\n",i1+1,filnam);
  i1 += 1;
  if(i1 > 1) i1 = 1;

    printf("ex OS_checkFilExist %d |%s|\n",i1,filnam);

  return i1;

}



//================================================================
  int OS_system (char *syscmd) {
//================================================================
// OS_system                  Perform OS-Command; wait for completion (system)
// - enclose all parameters with pathname with ""
// - enclose complete command also with ""
// os-function "system" CANNOT handle exename with blanks (does not accept
//   exename enclosed with "")

// returns 0=OK; -1=Error


  int     irc, i1;
  // char  sDir[256];
  BOOL    fExit;
  DWORD   dwExitCode, dw;
  HANDLE  hProcess,   hThread;
  PROCESS_INFORMATION    pi;
  STARTUPINFO sui;


  // printf("OS_system |%s|\n",syscmd);


  memset(&sui, 0, sizeof(sui));
  sui.cb = sizeof(sui);
  // sui.dwFlags     = STARTF_FORCEONFEEDBACK | STARTF_USESHOWWINDOW;
  // sui.wShowWindow = SW_SHOWNORMAL;

  if(!CreateProcess(
                NULL,                 // zu startende Appli (mit Pfad)
                syscmd,               // oder Commandline (appli=NULL)
                NULL,                 // Proc.Security
                NULL,                 // Thread security
                TRUE,                 // handle inheritance flag
                0, //HIGH_PRIORITY_CLASS,  // creation flags
                NULL,                 // pointer to new environment block
                NULL,   // sDir       // pointer to current directory name
                &sui,                 // pointer to STARTUPINFO
                &pi))  {              // pointer to PROCESS_INFORMATION

    irc = -1;
    goto L_exit;
  }



  // aufs beenden warten
  dw = WaitForSingleObject (pi.hProcess, INFINITE) ;


  irc = 0;

  if(dw != 0xFFFFFFFF) {
    // den Exitcode abfragen
    i1 = GetExitCodeProcess (pi.hProcess, &dwExitCode) ;
      // printf(" GetExitCodeProcess %d %d\n",i1,dwExitCode);
    if(dwExitCode) irc = -1;
  }

  // close the process and thread object handles
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

    // printf(" GetLastError = %d\n",GetLastError());

  L_exit:

    // printf(" ex-OS_system %d\n",irc);

  return irc;

}

#else
// Unix

//================================================================
  int OS_system (char *buf) {
//================================================================
// OS_system                  Perform OS-Command; wait for completion (system)
//   do not wait: use OS_exec

// Spezialversion fuer AIX + CATIA.


  void *catch;
  int ret;

  catch = signal(SIGCLD, SIG_DFL);   // SIGCLD | SIGCHLD

  ret = system(buf);

  signal(SIGCLD, catch);
  if (ret) { perror(buf); }

  return(ret);

}

#endif


///==========================================================
  void TX_Error (char* txt, ...) {
///==========================================================
/// formatted errortext into messagewindow.
/// Example:
///   TX_Print (".. distance must not exceed %lf", d1);


  char   TX_buf1[1024];


  va_list va;

  va_start(va,txt);


  strcpy(TX_buf1, "*** Fehler: ");
                // o123456789012
  vsprintf(&TX_buf1[12],txt,va);
  va_end(va);

  printf ("%s",TX_buf1);

}


// EOF
