#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetUuid)(
CUuuid *, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetUuid(CUuuid *uuid, CUdevice dev) {
		fprintf(stderr, "cuDeviceGetUuid()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetUuid)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetUuid = (CUresult (*)(
			CUuuid *, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetUuid");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetUuid():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceGetUuid(
		uuid, 
		dev
		);
	}
}