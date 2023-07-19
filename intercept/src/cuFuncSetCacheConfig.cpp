#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuFuncSetCacheConfig)(
CUfunction, 
CUfunc_cache
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuFuncSetCacheConfig(CUfunction hfunc, CUfunc_cache config) {
		fprintf(stderr, "cuFuncSetCacheConfig()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuFuncSetCacheConfig)
		{
			//fetch the original function addr using dlsym
			original_cuFuncSetCacheConfig = (CUresult (*)(
			CUfunction, 
			CUfunc_cache)
			) dlsym(original_libcuda_handle, "cuFuncSetCacheConfig");
			fprintf(stderr, "original_cuFuncSetCacheConfig:%p\n", original_cuFuncSetCacheConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuFuncSetCacheConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuFuncSetCacheConfig(
		hfunc, 
		config
		);
	}
}