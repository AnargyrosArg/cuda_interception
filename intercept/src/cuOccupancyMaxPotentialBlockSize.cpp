#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuOccupancyMaxPotentialBlockSize)(
int *, 
int *, 
CUfunction, 
CUoccupancyB2DSize, 
size_t, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuOccupancyMaxPotentialBlockSize(int *minGridSize, int *blockSize, CUfunction func, CUoccupancyB2DSize blockSizeToDynamicSMemSize, size_t dynamicSMemSize, int blockSizeLimit) {
		fprintf(stderr, "===============\ncuOccupancyMaxPotentialBlockSize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuOccupancyMaxPotentialBlockSize)
		{
			//fetch the original function addr using dlsym
			original_cuOccupancyMaxPotentialBlockSize = (CUresult (*)(
			int *, 
			int *, 
			CUfunction, 
			CUoccupancyB2DSize, 
			size_t, 
			int)
			) dlsym(original_libcuda_handle, "cuOccupancyMaxPotentialBlockSize");
			fprintf(stderr, "original_cuOccupancyMaxPotentialBlockSize:%p\n", original_cuOccupancyMaxPotentialBlockSize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuOccupancyMaxPotentialBlockSize():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuOccupancyMaxPotentialBlockSize(
		minGridSize, 
		blockSize, 
		func, 
		blockSizeToDynamicSMemSize, 
		dynamicSMemSize, 
		blockSizeLimit
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}