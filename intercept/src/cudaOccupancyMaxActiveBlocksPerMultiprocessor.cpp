#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaOccupancyMaxActiveBlocksPerMultiprocessor)(
int *, 
const void *, 
int, 
size_t
);
extern "C"
{
	cudaError_t cudaOccupancyMaxActiveBlocksPerMultiprocessor(int *numBlocks, const void *func, int blockSize, size_t dynamicSMemSize) {
		fprintf(stderr, "===============\ncudaOccupancyMaxActiveBlocksPerMultiprocessor()\n");
		char* __dlerror;
		if (!original_cudaOccupancyMaxActiveBlocksPerMultiprocessor)
		{
			//fetch the original function addr using dlsym
			original_cudaOccupancyMaxActiveBlocksPerMultiprocessor = (cudaError_t (*)(
			int *, 
			const void *, 
			int, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaOccupancyMaxActiveBlocksPerMultiprocessor");
			fprintf(stderr, "original_cudaOccupancyMaxActiveBlocksPerMultiprocessor:%p\n", original_cudaOccupancyMaxActiveBlocksPerMultiprocessor);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaOccupancyMaxActiveBlocksPerMultiprocessor():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaOccupancyMaxActiveBlocksPerMultiprocessor(
		numBlocks, 
		func, 
		blockSize, 
		dynamicSMemSize
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}