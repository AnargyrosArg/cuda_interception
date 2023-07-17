#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceComputeCapability)(
int *, 
int *, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceComputeCapability(int *major, int *minor, CUdevice dev) {
		fprintf(stderr, "cuDeviceComputeCapability()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceComputeCapability)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceComputeCapability = (CUresult (*)(
			int *, 
			int *, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceComputeCapability");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceComputeCapability():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceComputeCapability(
		major, 
		minor, 
		dev
		);
	}
}