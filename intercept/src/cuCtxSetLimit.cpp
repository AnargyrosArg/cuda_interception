#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxSetLimit)(
CUlimit, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxSetLimit(CUlimit limit, size_t value) {
		fprintf(stderr, "===============\ncuCtxSetLimit()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxSetLimit)
		{
			//fetch the original function addr using dlsym
			original_cuCtxSetLimit = (CUresult (*)(
			CUlimit, 
			size_t)
			) dlsym(original_libcuda_handle, "cuCtxSetLimit");
			fprintf(stderr, "original_cuCtxSetLimit:%p\n", original_cuCtxSetLimit);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxSetLimit():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuCtxSetLimit(
		limit, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}