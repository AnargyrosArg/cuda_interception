#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaChooseDevice)(
int *, 
const struct cudaDeviceProp *
);
extern "C"
{
	cudaError_t cudaChooseDevice(int *device, const struct cudaDeviceProp *prop) {
		fprintf(stderr, "===============\ncudaChooseDevice()\n");
		char* __dlerror;
		if (!original_cudaChooseDevice)
		{
			//fetch the original function addr using dlsym
			original_cudaChooseDevice = (cudaError_t (*)(
			int *, 
			const struct cudaDeviceProp *)
			) dlsym(RTLD_NEXT, "cudaChooseDevice");
			fprintf(stderr, "original_cudaChooseDevice:%p\n", original_cudaChooseDevice);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaChooseDevice():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaChooseDevice(
		device, 
		prop
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}