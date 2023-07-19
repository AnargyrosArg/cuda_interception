#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetExecAffinitySupport)(
int *, 
CUexecAffinityType, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetExecAffinitySupport(int *pi, CUexecAffinityType type, CUdevice dev) {
		fprintf(stderr, "cuDeviceGetExecAffinitySupport()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetExecAffinitySupport)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetExecAffinitySupport = (CUresult (*)(
			int *, 
			CUexecAffinityType, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetExecAffinitySupport");
			fprintf(stderr, "original_cuDeviceGetExecAffinitySupport:%p\n", original_cuDeviceGetExecAffinitySupport);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetExecAffinitySupport():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceGetExecAffinitySupport(
		pi, 
		type, 
		dev
		);
	}
}