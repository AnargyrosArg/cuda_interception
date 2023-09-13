#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags)(
int *, 
const void *, 
int, 
size_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags(int *numBlocks, const void *func, int blockSize, size_t dynamicSMemSize, unsigned int flags) {
		fprintf(stderr, "===============\ncudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags()\n");
		char* __dlerror;
		if (!original_cudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags = (cudaError_t (*)(
			int *, 
			const void *, 
			int, 
			size_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags");
			fprintf(stderr, "original_cudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags:%p\n", original_cudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaOccupancyMaxActiveBlocksPerMultiprocessorWithFlags(
		numBlocks, 
		func, 
		blockSize, 
		dynamicSMemSize, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}