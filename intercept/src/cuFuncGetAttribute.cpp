#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuFuncGetAttribute)(
int *, 
CUfunction_attribute, 
CUfunction
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuFuncGetAttribute(int *pi, CUfunction_attribute attrib, CUfunction hfunc) {
		fprintf(stderr, "cuFuncGetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuFuncGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuFuncGetAttribute = (CUresult (*)(
			int *, 
			CUfunction_attribute, 
			CUfunction)
			) dlsym(original_libcuda_handle, "cuFuncGetAttribute");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuFuncGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuFuncGetAttribute(
		pi, 
		attrib, 
		hfunc
		);
	}
}