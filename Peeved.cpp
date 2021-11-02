#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <python.h>
using namespace std;

#include "detours.h"

#define THISEXPORTS 
#if defined(THISEXPORTS)
#define KEYDLL3_API __declspec(dllexport)
#else
#define KEYDLL3_API __declspec(dllimport)
#endif
#pragma data_seg(".HOOKDATA")//Shared data among all instances.
HHOOK hook = NULL;

HINSTANCE hinsta = NULL;
#pragma data_seg()

//linker directive
#pragma comment(linker, "/SECTION:.HOOKDATA,RWS")

//This function installs the Keyboard hook:
KEYDLL3_API void installhook();

//This function removes the previously installed hook.
KEYDLL3_API void removehook();

//hook procedure:
KEYDLL3_API LRESULT CALLBACK hookproc( int ncode,
									  WPARAM wparam,
									  LPARAM lparam);



bool running = false;
bool adamloaded = false;
bool fileloaded =false;







HINSTANCE hinstance = NULL;
HANDLE threadhandle = NULL;

DWORD WINAPI threadfunc(LPVOID junk);







static LONG nExtends = 0;
static LONG nInterns = 0;

static ofstream myfile;


static char *(*TruePy_GetPath)() = NULL;


static int ( * TruePyRun_AnyFile)(	FILE *fp, const char *filename) = NULL;
static int ( * TruePyRun_AnyFileFlags)(	FILE *fp, const char *filename, void *flags) = NULL;
static int ( * TruePyRun_AnyFileEx)(	FILE *fp, const char *filename, int closeit) = NULL;
static int ( * TruePyRun_AnyFileExFlags)(	FILE *fp, const char *filename, int closeit, void *flags) = NULL;
static int ( * TruePyRun_SimpleString)(	const char *command) = NULL;
static int ( * TruePyRun_SimpleStringFlags)(	const char *command, void *flags) = NULL;
static int ( * TruePyRun_SimpleFile)(	FILE *fp, const char *filename) = NULL;
static int ( * TruePyRun_SimpleFileFlags)(	FILE *fp, const char *filename, void *flags) = NULL;
static int ( * TruePyRun_SimpleFileEx)(	FILE *fp, const char *filename, int closeit) = NULL;
static int ( * TruePyRun_SimpleFileExFlags)(	FILE *fp, const char *filename, int closeit, void *flags) = NULL;
static int ( * TruePyRun_InteractiveOne)(	FILE *fp, const char *filename) = NULL;
static int ( * TruePyRun_InteractiveOneFlags)(	FILE *fp, const char *filename, void *flags) = NULL;
static int ( * TruePyRun_InteractiveLoop)(	FILE *fp, const char *filename) = NULL;
static int ( * TruePyRun_InteractiveLoopFlags)(	FILE *fp, const char *filename, void *flags) = NULL;
static void* ( * TruePyParser_SimpleParseString)(	const char *str, int start) = NULL;
static void* ( * TruePyParser_SimpleParseStringFlags)(	 const char *str, int start, int flags) = NULL;
static void* ( * TruePyParser_SimpleParseStringFlagsFilename)(	 const char *str, const char *filename, int start, int flags) = NULL;
static void* ( * TruePyParser_SimpleParseFile)(	FILE *fp, const char *filename, int start) = NULL;
static void* ( * TruePyParser_SimpleParseFileFlags)(	FILE *fp, const char *filename, int start, int flags) = NULL;
static void* ( * TruePyRun_String)(	const char *str, int start, void *globals, void *locals) = NULL;
static void* ( * TruePyRun_StringFlags)(	const char *str, int start, void *globals, void *locals, void *flags) = NULL;
static void* ( * TruePyRun_File)(	FILE *fp, const char *filename, int start, void *globals, void *locals) = NULL;
static void* ( * TruePyRun_FileEx)(	FILE *fp, const char *filename, int start, void *globals, void *locals, int closeit) = NULL;
static void* ( * TruePyRun_FileFlags)(	FILE *fp, const char *filename, int start, void *globals, void *locals, void *flags) = NULL;
static void* ( * TruePyRun_FileExFlags)(	FILE *fp, const char *filename, int start, void *globals, void *locals, int closeit, void *flags) = NULL;
static void* ( * TruePy_CompileString)(	const char *str, const char *filename, int start) = NULL;
static void* ( * TruePy_CompileStringFlags)(	const char *str, const char *filename, int start, void *flags) = NULL;

static void* (*TruePyImport_ImportModule)(	const char *name) = NULL;
static void* (*TruePyImport_ImportModuleEx)(	char *name, void *globals, void *locals, void *fromlist) = NULL;

static void * (*TruePyString_FromString)(	char *name) = NULL;
static PyObject * (*TruePyImport_AddModule)(char* name) = NULL;
static PyObject* (*TruePyModule_GetDict)(	PyObject *module) = NULL;
static PyObject* (*TruePyDict_GetItemString)(	PyObject *p, const char *key) = NULL;

static int (*TruePyDict_Check)(	PyObject *p)=NULL;


static PyObject* (*TruePyDict_Keys)(	PyObject *p) = NULL;

static PyObject* (*TruePyList_GetItem)(	PyObject *list, Py_ssize_t index) = NULL;
static Py_ssize_t (*TruePyList_Size)(	PyObject *list) = NULL;

static char* (*TruePyString_AsString)(	PyObject *string) = NULL;

static int (*TruePyModule_AddObject)(	PyObject *module, const char *name, PyObject *value) = NULL;

static PyObject* (*TruePyObject_CallObject)(	PyObject *callable_object, PyObject *args) = NULL;

static int ( * TrueWinMain)(HINSTANCE hInstance,
							HINSTANCE hPrevInstance,
							LPSTR lpCmdLine,
							int nCmdShow) = NULL;


KEYDLL3_API void installhook()
{
	hook = NULL;

	hook = SetWindowsHookEx( WH_KEYBOARD,
		hookproc,
		hinsta,
		NULL);
	if(hook==NULL)
		MessageBox( NULL,
		L"Unable to install hook",
		L"Error!",
		MB_OK);
}

KEYDLL3_API void removehook()
{
	UnhookWindowsHookEx(hook);
}

KEYDLL3_API LRESULT CALLBACK hookproc( int ncode,
									  WPARAM wparam,
									  LPARAM lparam)
{
	if(wparam == VK_DIVIDE)
	{
		UnhookWindowsHookEx(hook);
	}
/*
	else if(wparam == VK_DIVIDE && adamloaded ==false)
	{
		adamloaded = true;
		MessageBox(0,L"adamadam.py", L"",MB_OK);
		FILE *fp2 =fopen("C:\\adamadam.py","r");
		TruePyRun_SimpleFile(fp2,"C:\\adamadam.py");

		Sleep(1000);
	}*/


	else if(wparam == VK_MULTIPLY && fileloaded == false)
	{
		fileloaded = true;
		//MessageBox(0,L"file.py", L"",MB_OK);
		FILE *fp2;
		fp2 =fopen("C:\\file.py","r");
		TruePyRun_SimpleFile(fp2,"C:\\file.py");
		Sleep(1000);

	}


	//pass control to next hook.
	return ( CallNextHookEx(hook,ncode,wparam,lparam) );
}


// Every detour DLL needs at least one export for use by setdll.exe or
// withdll.exe.  The export is never called, just referenced to force load.
VOID NullExport()
{
}
// 
static int CALLCOUNT = 0;
CRITICAL_SECTION cs;
static PyObject* myTruePyObject_CallObject(PyObject *callable_object, PyObject *args)
{


	return TruePyObject_CallObject(callable_object, args);
}
// static int myTruePyModule_AddObject(	PyObject *module, const char *name, PyObject *value)
// {
// 
// 	return TruePyModule_AddObject(module, name,value);
// }
// 
// static void * myTruePyString_FromString(	char *name)
// {
// 	return TruePyString_FromString(name);
// }
// static void* myTruePyImport_ImportModule(	const char *name)
// {
// 	return TruePyImport_ImportModule(name);
// }
// static void* myTruePyImport_ImportModuleEx(	char *name, void *globals, void *locals, void *fromlist)
// {
// 
// 	return TruePyImport_ImportModuleEx(name,globals,locals,fromlist);
// }
// 
// static int  myTruePyRun_AnyFile(	FILE *fp, const char *filename)
// {
// 	return TruePyRun_AnyFile(fp,filename);
// }
// static int   myTruePyRun_AnyFileFlags(	FILE *fp, const char *filename, void *flags)
// {
// 	return TruePyRun_AnyFileFlags(	fp, filename, flags);
// }
// static int   myTruePyRun_AnyFileEx(	FILE *fp, const char *filename, int closeit)
// {
// 	return TruePyRun_AnyFileEx(fp,filename, closeit);
// }
// static int   myTruePyRun_AnyFileExFlags(	FILE *fp, const char *filename, int closeit, void *flags)
// {
// 	return TruePyRun_AnyFileExFlags(fp, filename,  closeit, flags);
// }
// static int   myTruePyRun_SimpleString(	const char *command)
// {
// 	return TruePyRun_SimpleString(	command);
// }
// static int   myTruePyRun_SimpleStringFlags(	const char *command, void *flags)
// {
// 	return TruePyRun_SimpleStringFlags(	command, flags);
// }
// 
// static int   myTruePyRun_SimpleFileFlags(	FILE *fp, const char *filename, void *flags)
// {
// 	return TruePyRun_SimpleFileFlags(	fp, filename, flags);
// }
// static int   myTruePyRun_SimpleFileEx(	FILE *fp, const char *filename, int closeit)
// {
// 	return TruePyRun_SimpleFileEx(	fp, filename, closeit);
// }
// static int   myTruePyRun_SimpleFileExFlags(	FILE *fp, const char *filename, int closeit, void *flags)
// {
// 	return TruePyRun_SimpleFileExFlags(	fp, filename, closeit, flags);
// }
// static int   myTruePyRun_InteractiveOne(	FILE *fp, const char *filename)
// {
// 	return TruePyRun_InteractiveOne(fp, filename);
// }
// static int   myTruePyRun_InteractiveOneFlags(	FILE *fp, const char *filename, void *flags)
// {
// 	return TruePyRun_InteractiveOneFlags(fp, filename, flags);
// }
// static int   myTruePyRun_InteractiveLoop(	FILE *fp, const char *filename)
// {
// 	return TruePyRun_InteractiveLoop(fp, filename);
// }
// static int   myTruePyRun_InteractiveLoopFlags(	FILE *fp, const char *filename, void *flags)
// {
// 	return TruePyRun_InteractiveLoopFlags(	fp, filename, flags);
// }
// static void*   myTruePyParser_SimpleParseString(	const char *str, int start)
// {
// 	return TruePyParser_SimpleParseString(str, start);
// }
// static void*   myTruePyParser_SimpleParseStringFlags(	 const char *str, int start, int flags)
// {
// 	return  TruePyParser_SimpleParseStringFlags(str, start, flags);
// }
// static void*   myTruePyParser_SimpleParseStringFlagsFilename(	 const char *str, const char *filename, int start, int flags)
// {
// 	return TruePyParser_SimpleParseStringFlagsFilename(str, filename, start, flags);
// }
// static void*   myTruePyParser_SimpleParseFile(	FILE *fp, const char *filename, int start)
// {
// 	return TruePyParser_SimpleParseFile(fp, filename, start);
// }
// static void*   myTruePyParser_SimpleParseFileFlags(	FILE *fp, const char *filename, int start, int flags)
// {
// 	return TruePyParser_SimpleParseFileFlags(fp, filename, start, flags);
// }
// static void*   myTruePyRun_String(	const char *str, int start, void *globals, void *locals)
// {
// 	return TruePyRun_String(str, start, globals, locals);
// }
// static void*   myTruePyRun_StringFlags(	const char *str, int start, void *globals, void *locals, void *flags)
// {
// 	void* bla =   TruePyRun_StringFlags(	str, start, globals, locals, flags);
// 	return bla;
// }
// static void*   myTruePyRun_File(	FILE *fp, const char *filename, int start, void *globals, void *locals)
// {
// 	return TruePyRun_File(	fp, filename, start, globals, locals);
// }
// static void*   myTruePyRun_FileEx(	FILE *fp, const char *filename, int start, void *globals, void *locals, int closeit)
// {
// 	return TruePyRun_FileEx(	fp, filename, start, globals, locals, closeit);
// }
// static void*   myTruePyRun_FileFlags(	FILE *fp, const char *filename, int start, void *globals, void *locals, void *flags)
// {
// 	return TruePyRun_FileFlags(	fp, filename,  start, globals, locals, flags);
// }
// static void*   myTruePyRun_FileExFlags(	FILE *fp, const char *filename, int start, void *globals, void *locals, int closeit, void *flags)
// {
// 	return TruePyRun_FileExFlags(	fp, filename,  start, globals, locals, closeit, flags);
// }
// static void*   myTruePy_CompileString(	const char *str, const char *filename, int start)
// {
// 	return TruePy_CompileString(	str, filename,  start);
// }
// static void*   myTruePy_CompileStringFlags(	const char *str, const char *filename, int start, void *flags)
// {
// 	char* asdf = TruePy_GetPath();
// 	myfile << asdf<<endl;
// 	return TruePy_CompileStringFlags(	str, filename,  start, flags);
// }
// 
// static PyObject*   myTruePyImport_AddModule(	 char *str)
// {
// 	return TruePyImport_AddModule(	str);
// }


static int  ExtendWinMain(HINSTANCE hInstance,
						  HINSTANCE hPrevInstance,
						  LPSTR lpCmdLine,
						  int nCmdShow)
{
	// We couldn't call LoadLibrary in DllMain, so our functions here.
	LONG error;

	//MessageBox(0, L"Im a hooker...",L"...", MB_OK);
	// We separate out the functions in the export table (Target)




	TruePyRun_AnyFile =(int ( *)(	FILE *fp, const char *filename))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_AnyFile");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	error = DetourAttach(&(PVOID&)TruePyRun_AnyFile, myTruePyRun_AnyFile);

// 	if(error == ERROR_INVALID_HANDLE )
// 	{
// 		MessageBox(0, L"ERROR_INVALID_HANDLE", L"", MB_OK);
// 	}
// 	error = DetourTransactionCommit();
// 	if(error != NO_ERROR)
// 	{
// 		MessageBox(0, L"TruePyRun_AnyFile failed", L"", MB_OK);
// 	}


	TruePyRun_AnyFileFlags =(int ( *)(FILE *fp, const char *filename, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_AnyFileFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_AnyFileFlags, myTruePyRun_AnyFileFlags);
// 	error = DetourTransactionCommit();
// 	if(error != NO_ERROR)
// 	{
// 		MessageBox(0, L"TruePyRun_AnyFileFlags failed", L"", MB_OK);
// 	}


	TruePyRun_AnyFileEx =(int ( *)(FILE *fp, const char *filename, int closeit))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_AnyFileEx");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_AnyFileEx, myTruePyRun_AnyFileEx);
// 	error = DetourTransactionCommit();
// 	if(error != NO_ERROR)
// 	{
// 		MessageBox(0, L"TruePyRun_AnyFileEx failed", L"", MB_OK);
// 	}

	TruePyRun_AnyFileExFlags =(int ( *)(FILE *fp, const char *filename, int closeit, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_AnyFileExFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_AnyFileExFlags, myTruePyRun_AnyFileExFlags);
// 	error = DetourTransactionCommit();
// 	if(error != NO_ERROR)
// 	{
// 		MessageBox(0, L"TruePyRun_AnyFileExFlags failed", L"", MB_OK);
// 	}

	TruePyRun_SimpleString =(int ( *)(const char *command))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_SimpleString");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_SimpleString, myTruePyRun_SimpleString);
// 	error = DetourTransactionCommit();
// 	if(error != NO_ERROR)
// 	{
// 		MessageBox(0, L"TruePyRun_SimpleString failed", L"", MB_OK);
// 	}

	TruePyRun_SimpleStringFlags =(int ( *)(const char *command, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_SimpleStringFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_SimpleStringFlags, myTruePyRun_SimpleStringFlags);
// 	error = DetourTransactionCommit();
// 	if(error != NO_ERROR)
// 	{
// 		MessageBox(0, L"TruePyRun_SimpleStringFlags failed", L"", MB_OK);
// 	}

	TruePyRun_SimpleFile =(int ( *)(FILE *fp, const char *filename))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_SimpleFile");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_SimpleFile, myTruePyRun_SimpleFile);
// 	error = DetourTransactionCommit();
// 	if(error != NO_ERROR)
// 	{
// 		MessageBox(0, L"TruePyRun_SimpleFile failed", L"", MB_OK);
// 	}

	TruePyRun_SimpleFileFlags =(int ( *)(FILE *fp, const char *filename, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_SimpleFileFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_SimpleFileFlags, myTruePyRun_SimpleFileFlags);
// 	error = DetourTransactionCommit();

	TruePyRun_SimpleFileEx =(int ( *)(FILE *fp, const char *filename, int closeit))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_SimpleFileEx");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_SimpleFileEx, myTruePyRun_SimpleFileEx);
// 	error = DetourTransactionCommit();

	TruePyRun_SimpleFileExFlags =(int ( *)(FILE *fp, const char *filename, int closeit, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_SimpleFileExFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_SimpleFileExFlags, myTruePyRun_SimpleFileExFlags);
// 	error = DetourTransactionCommit();

	TruePyRun_InteractiveOne =(int ( *)(	FILE *fp, const char *filename))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_InteractiveOne");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_InteractiveOne, myTruePyRun_InteractiveOne);
// 	error = DetourTransactionCommit();

	TruePyRun_InteractiveOneFlags =(int ( *)(	FILE *fp, const char *filename, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_InteractiveOneFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_InteractiveOneFlags, myTruePyRun_InteractiveOneFlags);
// 	error = DetourTransactionCommit();

	TruePyRun_InteractiveLoop =(int ( *)(	FILE *fp, const char *filename))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_InteractiveLoop");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_InteractiveLoop, myTruePyRun_InteractiveLoop);
// 	error = DetourTransactionCommit();

	TruePyRun_InteractiveLoopFlags =(int ( *)(FILE *fp, const char *filename, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_InteractiveLoopFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_InteractiveLoopFlags, myTruePyRun_InteractiveLoopFlags);
// 	error = DetourTransactionCommit();

	TruePyParser_SimpleParseString =(void* ( *)(const char *str, int start))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyParser_SimpleParseString");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyParser_SimpleParseString, myTruePyParser_SimpleParseString);
// 	error = DetourTransactionCommit();

	TruePyParser_SimpleParseStringFlags =(void* ( *)(	 const char *str, int start, int flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyParser_SimpleParseStringFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyParser_SimpleParseStringFlags, myTruePyParser_SimpleParseStringFlags);
// 	error = DetourTransactionCommit();

	TruePyParser_SimpleParseStringFlagsFilename =(void* ( *)(	const char *str, const char *filename, int start, int flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyParser_SimpleParseStringFlagsFilename");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyParser_SimpleParseStringFlagsFilename, myTruePyParser_SimpleParseStringFlagsFilename);
// 	error = DetourTransactionCommit();

	TruePyParser_SimpleParseFile =(void* ( *)(FILE *fp, const char *filename, int start))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyParser_SimpleParseFile");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyParser_SimpleParseFile, myTruePyParser_SimpleParseFile);
// 	error = DetourTransactionCommit();

	TruePyParser_SimpleParseFileFlags =(void* ( *)(	FILE *fp, const char *filename, int start, int flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyParser_SimpleParseFileFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyParser_SimpleParseFileFlags, myTruePyParser_SimpleParseFileFlags);
// 	error = DetourTransactionCommit();

	TruePyRun_String =(void* ( *)(	const char *str, int start, void *globals, void *locals))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_String");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_String, myTruePyRun_String);
// 	error = DetourTransactionCommit();

	TruePyRun_StringFlags =(void* ( *)(	const char *str, int start, void *globals, void *locals, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_StringFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_StringFlags, myTruePyRun_StringFlags);
// 	error = DetourTransactionCommit();

	TruePyRun_File =(void* ( *)(	FILE *fp, const char *filename, int start, void *globals, void *locals))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyParser_SimpleParseFile");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_File, myTruePyRun_File);
// 	error = DetourTransactionCommit();

	TruePyRun_FileEx =(void* ( *)(	FILE *fp, const char *filename, int start, void *globals, void *locals, int closeit))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_FileEx");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_FileEx, myTruePyRun_FileEx);
// 	error = DetourTransactionCommit();

	TruePyRun_FileFlags =(void* ( *)(	FILE *fp, const char *filename, int start, void *globals, void *locals, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_FileFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_FileFlags, myTruePyRun_FileFlags);
// 	error = DetourTransactionCommit();

	TruePyRun_FileExFlags =(void* ( *)(	FILE *fp, const char *filename, int start, void *globals, void *locals, int closeit, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyRun_FileExFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyRun_FileExFlags, myTruePyRun_FileExFlags);
// 	error = DetourTransactionCommit();

	TruePy_CompileString =(void* ( *)(	const char *str, const char *filename, int start))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "Py_CompileString");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePy_CompileString, myTruePy_CompileString);
// 	error = DetourTransactionCommit();

	TruePy_CompileStringFlags =(void* ( *)(	const char *str, const char *filename, int start, void *flags))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "Py_CompileStringFlags");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePy_CompileStringFlags, myTruePy_CompileStringFlags);
// 	error = DetourTransactionCommit();


	TruePyImport_ImportModule =(void* ( *)(	const char *name))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyImport_ImportModule");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyImport_ImportModule, myTruePyImport_ImportModule);
// 	error = DetourTransactionCommit();

	TruePyImport_ImportModuleEx =(void* ( *)(	char *name, void *globals, void *locals, void *fromlist))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyImport_ImportModuleEx");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyImport_ImportModuleEx, myTruePyImport_ImportModuleEx);
// 	error = DetourTransactionCommit();

	TruePyString_FromString =(void* ( *)(	char *name))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyString_FromString");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyString_FromString, myTruePyString_FromString);
// 	error = DetourTransactionCommit();

	TruePyImport_AddModule =(PyObject* ( *)(	char *name))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyImport_AddModule");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyImport_AddModule, myTruePyImport_AddModule);
// 	error = DetourTransactionCommit();


	TruePyModule_AddObject =(int ( *)(	PyObject *module, const char *name, PyObject *value))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyModule_AddObject");
// 	DetourTransactionBegin();
// 	DetourUpdateThread(GetCurrentThread());
// 	DetourAttach(&(PVOID&)TruePyModule_AddObject, myTruePyModule_AddObject);
// 	error = DetourTransactionCommit();

	TruePyObject_CallObject =(PyObject* ( *)(	PyObject *callable_object, PyObject *args))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyObject_CallObject");
 	DetourTransactionBegin();
 	DetourUpdateThread(GetCurrentThread());
 	DetourAttach(&(PVOID&)TruePyObject_CallObject, myTruePyObject_CallObject);
 	error = DetourTransactionCommit();

	TruePy_GetPath =(char* ( *)(	))DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "Py_GetPath");
	TruePyModule_GetDict=(	PyObject* (*) (PyObject *module)) DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyModule_GetDict");
	TruePyDict_Check=(	int (*) (PyObject *dict)) DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyDict_Check");
	TruePyDict_Keys=(	PyObject* (*) (PyObject *dict)) DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyDict_Keys");
	TruePyDict_GetItemString=(	PyObject* (*) (PyObject *dict, const char*key)) DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyDict_GetItemString");
	TruePyList_GetItem=(	PyObject* (*) (PyObject *list, Py_ssize_t index)) DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyList_GetItem");
	TruePyList_Size=(	Py_ssize_t (*) (PyObject *list)) DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyList_Size");
	TruePyString_AsString=(	char* (*) (PyObject *string)) DetourFindFunction("C:\\Program Files\\CCP\\EVE\\bin\\python25.dll", "PyString_AsString");

	installhook();
	return TrueWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

BOOL  DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
	LONG error;
	(void)hinst;
	(void)reserved;

	if (dwReason == DLL_PROCESS_ATTACH) {
		hinsta = hinst;
		DetourRestoreAfterWith();
		myfile.open ("evelog.log");

		myfile << "[LOG]Starting dll" <<endl<<endl;

		LPDWORD bla= new DWORD;
		threadhandle = CreateThread(NULL, 0, threadfunc, NULL, 0, bla);
		if(threadhandle==NULL)
		{
			MessageBox( NULL,
			L"Unable to start thread",
			L"Error!",
			MB_OK);
			myfile << "Thread not !started" << endl;
		}
		else
		{
			myfile << "Thread started" << endl;
		}

		InitializeCriticalSection(&cs);

		// NB: DllMain can't call LoadLibrary, so we hook the app entry point.

		TrueWinMain = (int ( *)(HINSTANCE, HINSTANCE, LPSTR, int))
			DetourGetEntryPoint(NULL);

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueWinMain, ExtendWinMain);
		error = DetourTransactionCommit();
		if(error != NO_ERROR)
		{
			myfile << "[LOG]###############################################################" <<endl;
			myfile << "[LOG] Could not hook python25.dll!" <<endl;
			myfile << "[LOG]###############################################################" <<endl;
			myfile.close();
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH) {
		myfile << "[LOG]###############################################################" <<endl;
		myfile << "[LOG] Closing dll!" <<endl;
		myfile << "[LOG]###############################################################" <<endl;
		TerminateThread(threadhandle,1);

		DeleteCriticalSection(&cs);


		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());



		// Detach the entry point.
		DetourDetach(&(PVOID&)TrueWinMain, ExtendWinMain);
		error = DetourTransactionCommit();
	}
	return TRUE;
}

DWORD WINAPI threadfunc(LPVOID junk)
{
	return 1;
}