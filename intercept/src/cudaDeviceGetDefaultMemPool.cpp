#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetDefaultMemPool)(
cudaMemPool_t *, 
int
);
extern "C"
{
	cudaError_t cudaDeviceGetDefaultMemPool(cudaMemPool_t *memPool, int device) {
		fprintf(stderr, "===============\ncudaDeviceGetDefaultMemPool()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetDefaultMemPool)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetDefaultMemPool = (cudaError_t (*)(
			cudaMemPool_t *, 
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceGetDefaultMemPool");
			fprintf(stderr, "original_cudaDeviceGetDefaultMemPool:%p\n", original_cudaDeviceGetDefaultMemPool);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetDefaultMemPool():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetDefaultMemPool(
		memPool, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}