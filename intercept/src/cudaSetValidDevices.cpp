#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaSetValidDevices)(
int *, 
int
);
extern "C"
{
	cudaError_t cudaSetValidDevices(int *device_arr, int len) {
		fprintf(stderr, "===============\ncudaSetValidDevices()\n");
		char* __dlerror;
		if (!original_cudaSetValidDevices)
		{
			//fetch the original function addr using dlsym
			original_cudaSetValidDevices = (cudaError_t (*)(
			int *, 
			int)
			) dlsym(RTLD_NEXT, "cudaSetValidDevices");
			fprintf(stderr, "original_cudaSetValidDevices:%p\n", original_cudaSetValidDevices);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaSetValidDevices():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaSetValidDevices(
		device_arr, 
		len
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}