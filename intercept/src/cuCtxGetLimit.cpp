#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxGetLimit)(
size_t *, 
CUlimit
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxGetLimit(size_t *pvalue, CUlimit limit) {
		fprintf(stderr, "===============\ncuCtxGetLimit()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxGetLimit)
		{
			//fetch the original function addr using dlsym
			original_cuCtxGetLimit = (CUresult (*)(
			size_t *, 
			CUlimit)
			) dlsym(original_libcuda_handle, "cuCtxGetLimit");
			fprintf(stderr, "original_cuCtxGetLimit:%p\n", original_cuCtxGetLimit);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxGetLimit():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuCtxGetLimit(
		pvalue, 
		limit
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}