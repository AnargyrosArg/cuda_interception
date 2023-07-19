#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetUuid_v2)(
CUuuid *, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetUuid_v2(CUuuid *uuid, CUdevice dev) {
		fprintf(stderr, "cuDeviceGetUuid_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetUuid_v2)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetUuid_v2 = (CUresult (*)(
			CUuuid *, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetUuid_v2");
			fprintf(stderr, "original_cuDeviceGetUuid_v2:%p\n", original_cuDeviceGetUuid_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetUuid_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceGetUuid_v2(
		uuid, 
		dev
		);
	}
}