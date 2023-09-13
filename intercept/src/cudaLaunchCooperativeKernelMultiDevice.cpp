#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaLaunchCooperativeKernelMultiDevice)(
struct cudaLaunchParams *, 
unsigned int, 
unsigned int
);
extern "C"
{
	cudaError_t cudaLaunchCooperativeKernelMultiDevice(struct cudaLaunchParams *launchParamsList, unsigned int numDevices, unsigned int flags) {
		fprintf(stderr, "===============\ncudaLaunchCooperativeKernelMultiDevice()\n");
		char* __dlerror;
		if (!original_cudaLaunchCooperativeKernelMultiDevice)
		{
			//fetch the original function addr using dlsym
			original_cudaLaunchCooperativeKernelMultiDevice = (cudaError_t (*)(
			struct cudaLaunchParams *, 
			unsigned int, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaLaunchCooperativeKernelMultiDevice");
			fprintf(stderr, "original_cudaLaunchCooperativeKernelMultiDevice:%p\n", original_cudaLaunchCooperativeKernelMultiDevice);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaLaunchCooperativeKernelMultiDevice():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaLaunchCooperativeKernelMultiDevice(
		launchParamsList, 
		numDevices, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}