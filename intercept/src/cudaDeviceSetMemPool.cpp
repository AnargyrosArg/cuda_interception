#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceSetMemPool)(
int, 
cudaMemPool_t
);
extern "C"
{
	cudaError_t cudaDeviceSetMemPool(int device, cudaMemPool_t memPool) {
		fprintf(stderr, "===============\ncudaDeviceSetMemPool()\n");
		char* __dlerror;
		if (!original_cudaDeviceSetMemPool)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceSetMemPool = (cudaError_t (*)(
			int, 
			cudaMemPool_t)
			) dlsym(RTLD_NEXT, "cudaDeviceSetMemPool");
			fprintf(stderr, "original_cudaDeviceSetMemPool:%p\n", original_cudaDeviceSetMemPool);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceSetMemPool():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceSetMemPool(
		device, 
		memPool
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}