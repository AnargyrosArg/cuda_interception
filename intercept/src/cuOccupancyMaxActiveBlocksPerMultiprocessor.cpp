#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuOccupancyMaxActiveBlocksPerMultiprocessor)(
int *, 
CUfunction, 
int, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuOccupancyMaxActiveBlocksPerMultiprocessor(int *numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize) {
		fprintf(stderr, "===============\ncuOccupancyMaxActiveBlocksPerMultiprocessor()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuOccupancyMaxActiveBlocksPerMultiprocessor)
		{
			//fetch the original function addr using dlsym
			original_cuOccupancyMaxActiveBlocksPerMultiprocessor = (CUresult (*)(
			int *, 
			CUfunction, 
			int, 
			size_t)
			) dlsym(original_libcuda_handle, "cuOccupancyMaxActiveBlocksPerMultiprocessor");
			fprintf(stderr, "original_cuOccupancyMaxActiveBlocksPerMultiprocessor:%p\n", original_cuOccupancyMaxActiveBlocksPerMultiprocessor);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuOccupancyMaxActiveBlocksPerMultiprocessor():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuOccupancyMaxActiveBlocksPerMultiprocessor(
		numBlocks, 
		func, 
		blockSize, 
		dynamicSMemSize
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}