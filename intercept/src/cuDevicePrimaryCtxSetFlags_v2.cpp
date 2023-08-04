#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDevicePrimaryCtxSetFlags_v2)(
CUdevice, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDevicePrimaryCtxSetFlags_v2(CUdevice dev, unsigned int flags) {
		fprintf(stderr, "===============\ncuDevicePrimaryCtxSetFlags_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDevicePrimaryCtxSetFlags_v2)
		{
			//fetch the original function addr using dlsym
			original_cuDevicePrimaryCtxSetFlags_v2 = (CUresult (*)(
			CUdevice, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuDevicePrimaryCtxSetFlags_v2");
			fprintf(stderr, "original_cuDevicePrimaryCtxSetFlags_v2:%p\n", original_cuDevicePrimaryCtxSetFlags_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDevicePrimaryCtxSetFlags_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDevicePrimaryCtxSetFlags_v2(
		dev, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}