// Define the DynamicLibLoader class, this class is applied to
// load dynamic lib e.g. Win32 - *.dll, mac - *.dylib, Linux - *.so.
//
// Author: xus
// E-Mail: xushvai@gmail.com

#ifndef __DYNAMICLIBLOADER_H__
#define __DYNAMICLIBLOADER_H__

#include "os.h"

typedef void* (*Func)();

class DynamicLibLoader {
public:
	DynamicLibLoader();
	virtual~DynamicLibLoader();

	void Init(const char* filename);

	Func GetExportFuncPtr();

private:
	// Load dynamic lib.
	//
	// Return
	//   HMOUDLE - handle of the lib, load failed return NULL.
	HMODULE Load();

	// Unload dynamic lib.
	//
	// Return
	//   void
	void Free();
	
	// Get the address of export function.
	//
	// Parameters
	//   func_name - function export name.
	// Return
	//   void* - the address of the export function, return NULL if get failed.
	void* GetExportFuncAddress(const char* func_name);

private:
	const char* filename_;
	HMODULE library_handle_;
	Func export_function_ptr_;
};

#endif

