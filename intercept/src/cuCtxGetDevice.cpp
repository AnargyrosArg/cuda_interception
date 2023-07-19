#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxGetDevice)(
CUdevice *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxGetDevice(CUdevice *device) {
		fprintf(stderr, "cuCtxGetDevice()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxGetDevice)
		{
			//fetch the original function addr using dlsym
			original_cuCtxGetDevice = (CUresult (*)(
			CUdevice *)
			) dlsym(original_libcuda_handle, "cuCtxGetDevice");
			fprintf(stderr, "original_cuCtxGetDevice:%p\n", original_cuCtxGetDevice);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxGetDevice():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxGetDevice(
		device
		);
	}
}