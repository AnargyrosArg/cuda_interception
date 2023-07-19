#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetLuid)(
char *, 
unsigned int *, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetLuid(char *luid, unsigned int *deviceNodeMask, CUdevice dev) {
		fprintf(stderr, "cuDeviceGetLuid()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetLuid)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetLuid = (CUresult (*)(
			char *, 
			unsigned int *, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetLuid");
			fprintf(stderr, "original_cuDeviceGetLuid:%p\n", original_cuDeviceGetLuid);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetLuid():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceGetLuid(
		luid, 
		deviceNodeMask, 
		dev
		);
	}
}