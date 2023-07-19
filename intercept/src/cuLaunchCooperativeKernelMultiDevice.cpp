#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLaunchCooperativeKernelMultiDevice)(
CUDA_LAUNCH_PARAMS *, 
unsigned int, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLaunchCooperativeKernelMultiDevice(CUDA_LAUNCH_PARAMS *launchParamsList, unsigned int numDevices, unsigned int flags) {
		fprintf(stderr, "cuLaunchCooperativeKernelMultiDevice()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLaunchCooperativeKernelMultiDevice)
		{
			//fetch the original function addr using dlsym
			original_cuLaunchCooperativeKernelMultiDevice = (CUresult (*)(
			CUDA_LAUNCH_PARAMS *, 
			unsigned int, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuLaunchCooperativeKernelMultiDevice");
			fprintf(stderr, "original_cuLaunchCooperativeKernelMultiDevice:%p\n", original_cuLaunchCooperativeKernelMultiDevice);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLaunchCooperativeKernelMultiDevice():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLaunchCooperativeKernelMultiDevice(
		launchParamsList, 
		numDevices, 
		flags
		);
	}
}