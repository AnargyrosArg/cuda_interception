#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDevicePrimaryCtxReset_v2)(
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDevicePrimaryCtxReset_v2(CUdevice dev) {
		fprintf(stderr, "===============\ncuDevicePrimaryCtxReset_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDevicePrimaryCtxReset_v2)
		{
			//fetch the original function addr using dlsym
			original_cuDevicePrimaryCtxReset_v2 = (CUresult (*)(
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDevicePrimaryCtxReset_v2");
			fprintf(stderr, "original_cuDevicePrimaryCtxReset_v2:%p\n", original_cuDevicePrimaryCtxReset_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDevicePrimaryCtxReset_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDevicePrimaryCtxReset_v2(
		dev
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}