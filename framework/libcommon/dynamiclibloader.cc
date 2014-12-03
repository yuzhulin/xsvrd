// Implementation for DynamicLibLoader class.
//
// Author: xus
// E-Mail: xushvai@gmail.com

#include "dynamiclibloader.h"

#include <iostream>

DynamicLibLoader::DynamicLibLoader()
	: filename_(NULL), library_handle_(NULL), export_function_ptr_(NULL)
{
}

DynamicLibLoader::~DynamicLibLoader()
{

}

void DynamicLibLoader::Init(const char* filename)
{
	filename_ = filename;
}

Func DynamicLibLoader::GetExportFuncPtr()
{
	if (!export_function_ptr_) {
		if (Load()) {
			export_function_ptr_ = (Func)(GetExportFuncAddress("CreateObj"));
		} else {
			Free();
		}
	}
	return export_function_ptr_;
}

void* DynamicLibLoader::GetExportFuncAddress(const char* func_name)
{
#if (defined(_WIN32))
	void* func_address = GetProcAddress(library_handle_, func_name);
	if (!func_address) {
		std::clog << "GetProcAddress failed, error code("
			<< GetLastError() << ")" << std::endl;
	}
#elif (defined(__gnu_linux__))
#endif
	return func_address;
}

HMODULE DynamicLibLoader::Load()
{
#if (defined(_WIN32))
	library_handle_ = ::LoadLibrary(filename_);
	if (!library_handle_) {
		std::clog << "LoadLibrary failed, error code("
			<< GetLastError() << ")" << std::endl;
	}
#elif (defined(__gnu_linux__))
#endif
	return library_handle_;
}

void DynamicLibLoader::Free()
{
#if (defined(_WIN32))
	::FreeLibrary(library_handle_);
#elif
#endif
}