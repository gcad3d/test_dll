/* dll1.c
 Example dll using functions/datafields of main
 main_export and i1TestMain must be imported from main-exe.
*/

#if defined _MSC_VER || __MINGW64__
#include <windows.h>
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if defined _MSC_VER || __MINGW64__
// export this functions
__declspec(dllexport) int dll_main_start ();
__declspec(dllexport) int dll_main_exit ();
// import functions exported from core
#define extern __declspec(dllimport)
#endif


// import variable vom main-module
extern int i1TestMain;
extern int main_export (char *);



//================================================================
  int dll_main_start () {
//================================================================
// init plugin; use functions,variables of main

  printf(".. dll.. dll_main_start ..\n");

  // disp var of main-module
  printf(" i1TestMain = %d\n",i1TestMain);

  // call func in main-module
  main_export ("dll_main_start 1");

  return 0;

}


//================================================================
  int dll_main_exit () {
//================================================================
// do cleanup, close windows ..
// here unused

  printf("dll_main_exit ..\n");

//   printf(" i1TestMain = %d\n",i1TestMain);

//   main_export ("dll_main_exit 1");

  return 0;

}


// eof
