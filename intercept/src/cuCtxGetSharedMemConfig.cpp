#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxGetSharedMemConfig)(
CUsharedconfig *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxGetSharedMemConfig(CUsharedconfig *pConfig) {
		fprintf(stderr, "cuCtxGetSharedMemConfig()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxGetSharedMemConfig)
		{
			//fetch the original function addr using dlsym
			original_cuCtxGetSharedMemConfig = (CUresult (*)(
			CUsharedconfig *)
			) dlsym(original_libcuda_handle, "cuCtxGetSharedMemConfig");
			fprintf(stderr, "original_cuCtxGetSharedMemConfig:%p\n", original_cuCtxGetSharedMemConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxGetSharedMemConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxGetSharedMemConfig(
		pConfig
		);
	}
}