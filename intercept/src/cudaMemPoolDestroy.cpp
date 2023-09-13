#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolDestroy)(
cudaMemPool_t
);
extern "C"
{
	cudaError_t cudaMemPoolDestroy(cudaMemPool_t memPool) {
		fprintf(stderr, "===============\ncudaMemPoolDestroy()\n");
		char* __dlerror;
		if (!original_cudaMemPoolDestroy)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolDestroy = (cudaError_t (*)(
			cudaMemPool_t)
			) dlsym(RTLD_NEXT, "cudaMemPoolDestroy");
			fprintf(stderr, "original_cudaMemPoolDestroy:%p\n", original_cudaMemPoolDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolDestroy(
		memPool
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}