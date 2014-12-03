// Define the DynamicLibLoader class, this class is applied to
// load dynamic lib e.g.: Win32 - *.dll, mac - *.dylib, Linux - *.so.
//
// Author: xus
// E-Mail: xushvai@gmail.com

#ifndef __DYNAMICLIBLOADER_H__
#define __DYNAMICLIBLOADER_H__

#include "win_linux.h"

typedef void* (*Func)();

#if (defined(__gnu_linux__))
typedef void* HMODULE;
#endif

class DynamicLibLoader {
public:
	DynamicLibLoader();
	virtual~DynamicLibLoader();
	
	// Just set target dynamic file pathname.
	//
	// Parameters
	//   filename - pathname of the file(e.g."../lib/libtest.dll")
	// Return
	//   void - return nothing.
	void Init(const char* filename);

	// The target dynamic lib must export a function which 
	// just use for create a obj defined in the lib.
	//
	// Return
	//   void* - a void pointer to the object.
	void* CreateObjByExportFunction();

private:
	// Load dynamic lib.
	//
	// Return
	//   HMOUDLE - handle of the lib, load failed return NULL.
	HMODULE Load();

	// Unload dynamic lib.
	//
	// Return
	//   void - return nothing.
	void Free();
	
	// Get the address of export function.
	//
	// Parameters
	//   func_name - function export name.
	// Return
	//   void* - the address of the export function, return NULL if get failed.
	void* GetExportFuncAddress(const char* func_name);

private:
	const char* filename_;      // target dynamic lib file pathname
	HMODULE library_handle_;
	Func export_function_ptr_;
};

#endif

