// License: GPL
//
// Author: xus
// E-Mail: xushvai@gmail.com
//
// Overview:
// Define the DynamicLibLoader class, this class is applied to
// load dynamic lib e.g. Win32 - *.dll, mac - *.dylib, Linux - *.so.

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
	// Load dynamic lib
	//
	// Return
	//   handle of the lib, load failed return NULL
	HMODULE Load();

	void Free();
	
	// Get the address of export function
	//
	// Parameters
	//   func_name - function export name
	// Return
	//   return the address of the export function or NULL
	void* GetExportFuncAddress(const char* func_name);

private:
	const char* filename_;
	HMODULE library_handle_;
};

#endif

