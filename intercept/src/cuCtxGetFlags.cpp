#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxGetFlags)(
unsigned int *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxGetFlags(unsigned int *flags) {
		fprintf(stderr, "cuCtxGetFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxGetFlags)
		{
			//fetch the original function addr using dlsym
			original_cuCtxGetFlags = (CUresult (*)(
			unsigned int *)
			) dlsym(original_libcuda_handle, "cuCtxGetFlags");
			fprintf(stderr, "original_cuCtxGetFlags:%p\n", original_cuCtxGetFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxGetFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxGetFlags(
		flags
		);
	}
}