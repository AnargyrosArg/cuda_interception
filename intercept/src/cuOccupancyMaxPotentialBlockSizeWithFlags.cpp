#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuOccupancyMaxPotentialBlockSizeWithFlags)(
int *, 
int *, 
CUfunction, 
CUoccupancyB2DSize, 
size_t, 
int, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuOccupancyMaxPotentialBlockSizeWithFlags(int *minGridSize, int *blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit, unsigned int flags) {
		fprintf(stderr, "===============\ncuOccupancyMaxPotentialBlockSizeWithFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuOccupancyMaxPotentialBlockSizeWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cuOccupancyMaxPotentialBlockSizeWithFlags = (CUresult (*)(
			int *, 
			int *, 
			CUfunction, 
			CUoccupancyB2DSize, 
			size_t, 
			int, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuOccupancyMaxPotentialBlockSizeWithFlags");
			fprintf(stderr, "original_cuOccupancyMaxPotentialBlockSizeWithFlags:%p\n", original_cuOccupancyMaxPotentialBlockSizeWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuOccupancyMaxPotentialBlockSizeWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuOccupancyMaxPotentialBlockSizeWithFlags(
		minGridSize, 
		blockSize, 
		func, 
		blockSizeToDynamicSMemSize, 
		dynamicSMemSize, 
		blockSizeLimit, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}