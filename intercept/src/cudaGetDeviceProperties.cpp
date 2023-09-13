#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetDeviceProperties)(
struct cudaDeviceProp *, 
int
);
extern "C"
{
	cudaError_t cudaGetDeviceProperties(struct cudaDeviceProp *prop, int device) {
		fprintf(stderr, "===============\ncudaGetDeviceProperties()\n");
		char* __dlerror;
		if (!original_cudaGetDeviceProperties)
		{
			//fetch the original function addr using dlsym
			original_cudaGetDeviceProperties = (cudaError_t (*)(
			struct cudaDeviceProp *, 
			int)
			) dlsym(RTLD_NEXT, "cudaGetDeviceProperties");
			fprintf(stderr, "original_cudaGetDeviceProperties:%p\n", original_cudaGetDeviceProperties);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetDeviceProperties():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetDeviceProperties(
		prop, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}