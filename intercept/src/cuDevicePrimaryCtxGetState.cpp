#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDevicePrimaryCtxGetState)(
CUdevice, 
unsigned int *, 
int *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDevicePrimaryCtxGetState(CUdevice dev, unsigned int *flags, int *active) {
		fprintf(stderr, "===============\ncuDevicePrimaryCtxGetState()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDevicePrimaryCtxGetState)
		{
			//fetch the original function addr using dlsym
			original_cuDevicePrimaryCtxGetState = (CUresult (*)(
			CUdevice, 
			unsigned int *, 
			int *)
			) dlsym(original_libcuda_handle, "cuDevicePrimaryCtxGetState");
			fprintf(stderr, "original_cuDevicePrimaryCtxGetState:%p\n", original_cuDevicePrimaryCtxGetState);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDevicePrimaryCtxGetState():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDevicePrimaryCtxGetState(
		dev, 
		flags, 
		active
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}