#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaOccupancyAvailableDynamicSMemPerBlock)(
size_t *, 
const void *, 
int, 
int
);
extern "C"
{
	cudaError_t cudaOccupancyAvailableDynamicSMemPerBlock(size_t *dynamicSmemSize, const void *func, int numBlocks, int blockSize) {
		fprintf(stderr, "===============\ncudaOccupancyAvailableDynamicSMemPerBlock()\n");
		char* __dlerror;
		if (!original_cudaOccupancyAvailableDynamicSMemPerBlock)
		{
			//fetch the original function addr using dlsym
			original_cudaOccupancyAvailableDynamicSMemPerBlock = (cudaError_t (*)(
			size_t *, 
			const void *, 
			int, 
			int)
			) dlsym(RTLD_NEXT, "cudaOccupancyAvailableDynamicSMemPerBlock");
			fprintf(stderr, "original_cudaOccupancyAvailableDynamicSMemPerBlock:%p\n", original_cudaOccupancyAvailableDynamicSMemPerBlock);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaOccupancyAvailableDynamicSMemPerBlock():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaOccupancyAvailableDynamicSMemPerBlock(
		dynamicSmemSize, 
		func, 
		numBlocks, 
		blockSize
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}