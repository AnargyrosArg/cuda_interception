#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetByPCIBusId)(
CUdevice *, 
const char *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetByPCIBusId(CUdevice *dev, const char *pciBusId) {
		fprintf(stderr, "cuDeviceGetByPCIBusId()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetByPCIBusId)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetByPCIBusId = (CUresult (*)(
			CUdevice *, 
			const char *)
			) dlsym(original_libcuda_handle, "cuDeviceGetByPCIBusId");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetByPCIBusId():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceGetByPCIBusId(
		dev, 
		pciBusId
		);
	}
}