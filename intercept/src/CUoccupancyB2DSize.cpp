/*#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
size_t (*original_CUoccupancyB2DSize)(
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	// size_t CUoccupancyB2DSize(int blockSize) {
		fprintf(stderr, "===============\nCUoccupancyB2DSize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_CUoccupancyB2DSize)
		{
			//fetch the original function addr using dlsym
			original_CUoccupancyB2DSize = (size_t (*)(
			int)
			) dlsym(original_libcuda_handle, "CUoccupancyB2DSize");
			fprintf(stderr, "original_CUoccupancyB2DSize:%p\n", original_CUoccupancyB2DSize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function CUoccupancyB2DSize():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_CUoccupancyB2DSize(
		blockSize
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}
*/
