#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxSetCacheConfig)(
CUfunc_cache
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxSetCacheConfig(CUfunc_cache config) {
		fprintf(stderr, "cuCtxSetCacheConfig()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxSetCacheConfig)
		{
			//fetch the original function addr using dlsym
			original_cuCtxSetCacheConfig = (CUresult (*)(
			CUfunc_cache)
			) dlsym(original_libcuda_handle, "cuCtxSetCacheConfig");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxSetCacheConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxSetCacheConfig(
		config
		);
	}
}