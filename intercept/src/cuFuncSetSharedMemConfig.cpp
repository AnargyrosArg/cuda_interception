#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuFuncSetSharedMemConfig)(
CUfunction, 
CUsharedconfig
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuFuncSetSharedMemConfig(CUfunction hfunc, CUsharedconfig config) {
		fprintf(stderr, "cuFuncSetSharedMemConfig()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuFuncSetSharedMemConfig)
		{
			//fetch the original function addr using dlsym
			original_cuFuncSetSharedMemConfig = (CUresult (*)(
			CUfunction, 
			CUsharedconfig)
			) dlsym(original_libcuda_handle, "cuFuncSetSharedMemConfig");
			fprintf(stderr, "original_cuFuncSetSharedMemConfig:%p\n", original_cuFuncSetSharedMemConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuFuncSetSharedMemConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuFuncSetSharedMemConfig(
		hfunc, 
		config
		);
	}
}