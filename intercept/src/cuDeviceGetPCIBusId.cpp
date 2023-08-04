#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetPCIBusId)(
char *, 
int, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetPCIBusId(char *pciBusId, int len, CUdevice dev) {
		fprintf(stderr, "===============\ncuDeviceGetPCIBusId()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetPCIBusId)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetPCIBusId = (CUresult (*)(
			char *, 
			int, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetPCIBusId");
			fprintf(stderr, "original_cuDeviceGetPCIBusId:%p\n", original_cuDeviceGetPCIBusId);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetPCIBusId():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDeviceGetPCIBusId(
		pciBusId, 
		len, 
		dev
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}