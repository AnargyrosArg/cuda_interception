#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceSetLimit)(
enum cudaLimit, 
size_t
);
extern "C"
{
	cudaError_t cudaDeviceSetLimit(enum cudaLimit limit, size_t value) {
		fprintf(stderr, "===============\ncudaDeviceSetLimit()\n");
		char* __dlerror;
		if (!original_cudaDeviceSetLimit)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceSetLimit = (cudaError_t (*)(
			enum cudaLimit, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaDeviceSetLimit");
			fprintf(stderr, "original_cudaDeviceSetLimit:%p\n", original_cudaDeviceSetLimit);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceSetLimit():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceSetLimit(
		limit, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}