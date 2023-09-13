#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaSetDevice)(
int
);
extern "C"
{
	cudaError_t cudaSetDevice(int device) {
		fprintf(stderr, "===============\ncudaSetDevice()\n");
		char* __dlerror;
		if (!original_cudaSetDevice)
		{
			//fetch the original function addr using dlsym
			original_cudaSetDevice = (cudaError_t (*)(
			int)
			) dlsym(RTLD_NEXT, "cudaSetDevice");
			fprintf(stderr, "original_cudaSetDevice:%p\n", original_cudaSetDevice);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaSetDevice():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaSetDevice(
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}