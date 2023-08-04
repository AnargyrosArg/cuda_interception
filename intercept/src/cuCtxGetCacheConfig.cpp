#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxGetCacheConfig)(
CUfunc_cache *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxGetCacheConfig(CUfunc_cache *pconfig) {
		fprintf(stderr, "===============\ncuCtxGetCacheConfig()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxGetCacheConfig)
		{
			//fetch the original function addr using dlsym
			original_cuCtxGetCacheConfig = (CUresult (*)(
			CUfunc_cache *)
			) dlsym(original_libcuda_handle, "cuCtxGetCacheConfig");
			fprintf(stderr, "original_cuCtxGetCacheConfig:%p\n", original_cuCtxGetCacheConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxGetCacheConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuCtxGetCacheConfig(
		pconfig
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}