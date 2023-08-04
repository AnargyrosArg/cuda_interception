#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags)(
int *, 
CUfunction, 
int, 
size_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags(int *numBlocks, CUfunction func, int blockSize, size_t dynamicSMemSize, unsigned int flags) {
		fprintf(stderr, "===============\ncuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags = (CUresult (*)(
			int *, 
			CUfunction, 
			int, 
			size_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags");
			fprintf(stderr, "original_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags:%p\n", original_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags(
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