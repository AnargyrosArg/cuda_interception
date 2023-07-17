#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDevicePrimaryCtxRelease)(
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDevicePrimaryCtxRelease(CUdevice dev) {
		fprintf(stderr, "cuDevicePrimaryCtxRelease()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDevicePrimaryCtxRelease)
		{
			//fetch the original function addr using dlsym
			original_cuDevicePrimaryCtxRelease = (CUresult (*)(
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDevicePrimaryCtxRelease");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDevicePrimaryCtxRelease():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDevicePrimaryCtxRelease(
		dev
		);
	}
}