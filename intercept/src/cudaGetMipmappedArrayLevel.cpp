#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetMipmappedArrayLevel)(
cudaArray_t *, 
cudaMipmappedArray_const_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaGetMipmappedArrayLevel(cudaArray_t *levelArray, cudaMipmappedArray_const_t mipmappedArray, unsigned int level) {
		fprintf(stderr, "===============\ncudaGetMipmappedArrayLevel()\n");
		char* __dlerror;
		if (!original_cudaGetMipmappedArrayLevel)
		{
			//fetch the original function addr using dlsym
			original_cudaGetMipmappedArrayLevel = (cudaError_t (*)(
			cudaArray_t *, 
			cudaMipmappedArray_const_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaGetMipmappedArrayLevel");
			fprintf(stderr, "original_cudaGetMipmappedArrayLevel:%p\n", original_cudaGetMipmappedArrayLevel);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetMipmappedArrayLevel():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetMipmappedArrayLevel(
		levelArray, 
		mipmappedArray, 
		level
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}