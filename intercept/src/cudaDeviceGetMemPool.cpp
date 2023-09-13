#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetMemPool)(
cudaMemPool_t *, 
int
);
extern "C"
{
	cudaError_t cudaDeviceGetMemPool(cudaMemPool_t *memPool, int device) {
		fprintf(stderr, "===============\ncudaDeviceGetMemPool()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetMemPool)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetMemPool = (cudaError_t (*)(
			cudaMemPool_t *, 
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceGetMemPool");
			fprintf(stderr, "original_cudaDeviceGetMemPool:%p\n", original_cudaDeviceGetMemPool);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetMemPool():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetMemPool(
		memPool, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}