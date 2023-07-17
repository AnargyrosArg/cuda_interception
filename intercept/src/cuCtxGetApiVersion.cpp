#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxGetApiVersion)(
CUcontext, 
unsigned int *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxGetApiVersion(CUcontext ctx, unsigned int *version) {
		fprintf(stderr, "cuCtxGetApiVersion()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxGetApiVersion)
		{
			//fetch the original function addr using dlsym
			original_cuCtxGetApiVersion = (CUresult (*)(
			CUcontext, 
			unsigned int *)
			) dlsym(original_libcuda_handle, "cuCtxGetApiVersion");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxGetApiVersion():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxGetApiVersion(
		ctx, 
		version
		);
	}
}