#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLaunch)(
CUfunction
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLaunch(CUfunction f) {
		fprintf(stderr, "cuLaunch()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLaunch)
		{
			//fetch the original function addr using dlsym
			original_cuLaunch = (CUresult (*)(
			CUfunction)
			) dlsym(original_libcuda_handle, "cuLaunch");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLaunch():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLaunch(
		f
		);
	}
}