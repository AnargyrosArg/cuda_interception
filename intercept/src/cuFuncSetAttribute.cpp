#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuFuncSetAttribute)(
CUfunction, 
CUfunction_attribute, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuFuncSetAttribute(CUfunction hfunc, CUfunction_attribute attrib, int value) {
		fprintf(stderr, "cuFuncSetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuFuncSetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuFuncSetAttribute = (CUresult (*)(
			CUfunction, 
			CUfunction_attribute, 
			int)
			) dlsym(original_libcuda_handle, "cuFuncSetAttribute");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuFuncSetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuFuncSetAttribute(
		hfunc, 
		attrib, 
		value
		);
	}
}