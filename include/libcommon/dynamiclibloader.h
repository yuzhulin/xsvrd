// Define the DynamicLibLoader class, this class is applied to
// load dynamic lib e.g.: Win32 - *.dll, mac - *.dylib, Linux - *.so.
//
// Author: xus
// E-Mail: xushvai@gmail.com

#ifndef __DYNAMICLIBLOADER_H__
#define __DYNAMICLIBLOADER_H__

#include "os_refactor.h"

typedef void* (*Func)();

class DynamicLibLoader {
public:
	DynamicLibLoader();
	virtual~DynamicLibLoader();
	
	// Just set target dynamic file pathname and extend
	// name, you should set filename to "../lib/libtest"
	// and set extend to ".dll" if you want load "../lib/libtest.dll" 
	//
	// Parameters
	//   filename - pathname of the file(e.g.: "../lib/libtest")
	//   extend   - the file's extend name(e.g.: ".dll")
	// Return
	//   void - return nothing.
	void Init(const char* filename, const char* extend);

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
	//   symbol - function export name.
	// Return
	//   void* - the address of the export function, return NULL if get failed.
	void* GetExportFuncAddress(const char* symbol);

private:
	char filename_[MAX_PATH];      // target dynamic lib file pathname
	HMODULE library_handle_;
	Func export_function_ptr_;
};

#endif

