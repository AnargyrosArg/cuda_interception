#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGraphMemTrim)(
int
);
extern "C"
{
	cudaError_t cudaDeviceGraphMemTrim(int device) {
		fprintf(stderr, "===============\ncudaDeviceGraphMemTrim()\n");
		char* __dlerror;
		if (!original_cudaDeviceGraphMemTrim)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGraphMemTrim = (cudaError_t (*)(
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceGraphMemTrim");
			fprintf(stderr, "original_cudaDeviceGraphMemTrim:%p\n", original_cudaDeviceGraphMemTrim);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGraphMemTrim():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGraphMemTrim(
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}