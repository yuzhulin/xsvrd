// License: GPL
//
// Author: xus
// E-Mail: xushvai@gmail.com
//
// Overview:
// Define the DynamicLibLoader class, this class is applied to
// load dynamic lib e.g. Win32 - *.dll, mac - *.dylib, Linux - *.so.

#ifndef __DYNAMICLIBLOADER__
#define __DYNAMICLIBLOADER__

#include "os.h"

class DynamicLibLoader {
public:
	DynamicLibLoader();
	virtual~DynamicLibLoader();

	HMODULE Load();

	void Free();
};

#endif

