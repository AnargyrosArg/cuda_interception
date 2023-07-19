#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDevicePrimaryCtxRetain)(
CUcontext *, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDevicePrimaryCtxRetain(CUcontext *pctx, CUdevice dev) {
		fprintf(stderr, "cuDevicePrimaryCtxRetain()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDevicePrimaryCtxRetain)
		{
			//fetch the original function addr using dlsym
			original_cuDevicePrimaryCtxRetain = (CUresult (*)(
			CUcontext *, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDevicePrimaryCtxRetain");
			fprintf(stderr, "original_cuDevicePrimaryCtxRetain:%p\n", original_cuDevicePrimaryCtxRetain);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDevicePrimaryCtxRetain():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDevicePrimaryCtxRetain(
		pctx, 
		dev
		);
	}
}