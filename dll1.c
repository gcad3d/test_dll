/* dll1.c
 Example dll using functions/datafields of main
 main_export and i1TestMain must be imported from main-exe.
*/

#if defined _MSC_VER || __MINGW64__
#include <windows.h>
#endif


// set export " __declspec(dllexport)" or ""
#if defined _MSC_VER || __MINGW64__
#define export __declspec(dllexport)
#else
#define export
#endif


// set import " __declspec(dllimport)" or "extern"
#if defined _MSC_VER || __MINGW64__
#define import __declspec(dllimport)
#else
#define import extern
#endif



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




//----------------------------------------------------------------
// EXPORTS to main-module
export int dll_main_start ();
export int dll_main_exit ();


//----------------------------------------------------------------
// IMPORTS from main-module
import int i1TestMain;
import int main_export (char *);



//----------------------------------------------------------------
static int dll_i1 = 0;



//================================================================
  int dll_main_start () {
//================================================================
// init plugin; use functions,variables of main

  printf("\n.. dll.. dll_main_start ..\n");

  // disp var of main-module
  printf(" i1TestMain = %d dll_i1=%d\n",i1TestMain,dll_i1);
  ++i1TestMain;
  ++dll_i1;

  // call func in main-module
  main_export ("dll_main_start 1");
  printf("\n");

  return 0;

}


//================================================================
  int dll_main_exit () {
//================================================================
// do cleanup, close windows ..
// here unused

  printf("\n.. dll.. dll_main_exit ..\n");

  // disp var of main-module
  printf(" i1TestMain = %d dll_i1=%d\n",i1TestMain,dll_i1);

  main_export ("dll_main_exit 1");
  printf("\n");


  return 0;

}


// eof
