#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetProperties)(
CUdevprop *, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetProperties(CUdevprop *prop, CUdevice dev) {
		fprintf(stderr, "cuDeviceGetProperties()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetProperties)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetProperties = (CUresult (*)(
			CUdevprop *, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetProperties");
			fprintf(stderr, "original_cuDeviceGetProperties:%p\n", original_cuDeviceGetProperties);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetProperties():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceGetProperties(
		prop, 
		dev
		);
	}
}