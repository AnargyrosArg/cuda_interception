#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolTrimTo)(
cudaMemPool_t, 
size_t
);
extern "C"
{
	cudaError_t cudaMemPoolTrimTo(cudaMemPool_t memPool, size_t minBytesToKeep) {
		fprintf(stderr, "===============\ncudaMemPoolTrimTo()\n");
		char* __dlerror;
		if (!original_cudaMemPoolTrimTo)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolTrimTo = (cudaError_t (*)(
			cudaMemPool_t, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMemPoolTrimTo");
			fprintf(stderr, "original_cudaMemPoolTrimTo:%p\n", original_cudaMemPoolTrimTo);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolTrimTo():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolTrimTo(
		memPool, 
		minBytesToKeep
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}