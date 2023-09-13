#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetLimit)(
size_t *, 
enum cudaLimit
);
extern "C"
{
	cudaError_t cudaDeviceGetLimit(size_t *pValue, enum cudaLimit limit) {
		fprintf(stderr, "===============\ncudaDeviceGetLimit()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetLimit)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetLimit = (cudaError_t (*)(
			size_t *, 
			enum cudaLimit)
			) dlsym(RTLD_NEXT, "cudaDeviceGetLimit");
			fprintf(stderr, "original_cudaDeviceGetLimit:%p\n", original_cudaDeviceGetLimit);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetLimit():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetLimit(
		pValue, 
		limit
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}