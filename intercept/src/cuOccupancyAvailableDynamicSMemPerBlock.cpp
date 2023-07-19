#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuOccupancyAvailableDynamicSMemPerBlock)(
size_t *, 
CUfunction, 
int, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuOccupancyAvailableDynamicSMemPerBlock(size_t *dynamicSmemSize, CUfunction func, int numBlocks, int blockSize) {
		fprintf(stderr, "cuOccupancyAvailableDynamicSMemPerBlock()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuOccupancyAvailableDynamicSMemPerBlock)
		{
			//fetch the original function addr using dlsym
			original_cuOccupancyAvailableDynamicSMemPerBlock = (CUresult (*)(
			size_t *, 
			CUfunction, 
			int, 
			int)
			) dlsym(original_libcuda_handle, "cuOccupancyAvailableDynamicSMemPerBlock");
			fprintf(stderr, "original_cuOccupancyAvailableDynamicSMemPerBlock:%p\n", original_cuOccupancyAvailableDynamicSMemPerBlock);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuOccupancyAvailableDynamicSMemPerBlock():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuOccupancyAvailableDynamicSMemPerBlock(
		dynamicSmemSize, 
		func, 
		numBlocks, 
		blockSize
		);
	}
}