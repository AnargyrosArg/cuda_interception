#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxSynchronize)(
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxSynchronize() {
		fprintf(stderr, "cuCtxSynchronize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxSynchronize)
		{
			//fetch the original function addr using dlsym
			original_cuCtxSynchronize = (CUresult (*)(
			void)
			) dlsym(original_libcuda_handle, "cuCtxSynchronize");
			fprintf(stderr, "original_cuCtxSynchronize:%p\n", original_cuCtxSynchronize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxSynchronize():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxSynchronize(
		);
	}
}