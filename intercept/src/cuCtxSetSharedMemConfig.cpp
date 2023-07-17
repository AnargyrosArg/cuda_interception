#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxSetSharedMemConfig)(
CUsharedconfig
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxSetSharedMemConfig(CUsharedconfig config) {
		fprintf(stderr, "cuCtxSetSharedMemConfig()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxSetSharedMemConfig)
		{
			//fetch the original function addr using dlsym
			original_cuCtxSetSharedMemConfig = (CUresult (*)(
			CUsharedconfig)
			) dlsym(original_libcuda_handle, "cuCtxSetSharedMemConfig");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxSetSharedMemConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxSetSharedMemConfig(
		config
		);
	}
}