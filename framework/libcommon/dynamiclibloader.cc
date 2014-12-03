// Implementation for DynamicLibLoader class.
//
// Author: xus
// E-Mail: xushvai@gmail.com

#include "dynamiclibloader.h"

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

void* DynamicLibLoader::CreateObjByExportFunction()
{
	if (!export_function_ptr_) {
		if (Load()) {
			export_function_ptr_ = (Func)(GetExportFuncAddress("CreateObj"));
		} else {
			Free();
		}
	}
	return export_function_ptr_();
}

void* DynamicLibLoader::GetExportFuncAddress(const char* symbol)
{
	void* func_address = NULL;
#if (defined(_WIN32))
	func_address = GetProcAddress(library_handle_, symbol);
	if (!func_address) {
		std::cerr << "GetProcAddress failed, error code("
			<< GetLastError() << ")" << std::endl;
	}
#elif (defined(__gnu_linux__))
	func_address = dlsym(library_handle_, symbol);
	if (!func_address) {
		std::cerr << "dlsym failed, " << dlerror() << std::endl;
	}
#endif
	return func_address;
}

HMODULE DynamicLibLoader::Load()
{
#if (defined(_WIN32))
	library_handle_ = ::LoadLibrary(filename_);
	if (!library_handle_) {
		std::cerr << "LoadLibrary failed, error code("
			<< GetLastError() << ")" << std::endl;
	}
#elif (defined(__gnu_linux__))
	library_handle_ = dlopen(filename_, RTLD_NOW);
	if (!library_handle_) {
		std::cerr << "dlopen failed, " << dlerror() << std::endl;
	}
#endif
	return library_handle_;
}

void DynamicLibLoader::Free()
{
#if (defined(_WIN32))
	::FreeLibrary(library_handle_);
#elif (defined(__gnu_linux__))
	dlclose(library_handle_);
#endif
	library_handle_ = NULL;
}
