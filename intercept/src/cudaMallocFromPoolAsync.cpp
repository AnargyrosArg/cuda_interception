#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMallocFromPoolAsync)(
void **, 
size_t, 
cudaMemPool_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMallocFromPoolAsync(void **ptr, size_t size, cudaMemPool_t memPool, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMallocFromPoolAsync()\n");
		char* __dlerror;
		if (!original_cudaMallocFromPoolAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMallocFromPoolAsync = (cudaError_t (*)(
			void **, 
			size_t, 
			cudaMemPool_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMallocFromPoolAsync");
			fprintf(stderr, "original_cudaMallocFromPoolAsync:%p\n", original_cudaMallocFromPoolAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMallocFromPoolAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMallocFromPoolAsync(
		ptr, 
		size, 
		memPool, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}