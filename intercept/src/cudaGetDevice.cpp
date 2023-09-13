#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetDevice)(
int *
);
extern "C"
{
	cudaError_t cudaGetDevice(int *device) {
		fprintf(stderr, "===============\ncudaGetDevice()\n");
		char* __dlerror;
		if (!original_cudaGetDevice)
		{
			//fetch the original function addr using dlsym
			original_cudaGetDevice = (cudaError_t (*)(
			int *)
			) dlsym(RTLD_NEXT, "cudaGetDevice");
			fprintf(stderr, "original_cudaGetDevice:%p\n", original_cudaGetDevice);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetDevice():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetDevice(
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}