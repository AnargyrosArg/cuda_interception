#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMallocMipmappedArray)(
cudaMipmappedArray_t *, 
const struct cudaChannelFormatDesc *, 
struct cudaExtent, 
unsigned int, 
unsigned int
);
extern "C"
{
	cudaError_t cudaMallocMipmappedArray(cudaMipmappedArray_t *mipmappedArray, const struct cudaChannelFormatDesc *desc, struct cudaExtent extent, unsigned int numLevels, unsigned int flags) {
		fprintf(stderr, "===============\ncudaMallocMipmappedArray()\n");
		char* __dlerror;
		if (!original_cudaMallocMipmappedArray)
		{
			//fetch the original function addr using dlsym
			original_cudaMallocMipmappedArray = (cudaError_t (*)(
			cudaMipmappedArray_t *, 
			const struct cudaChannelFormatDesc *, 
			struct cudaExtent, 
			unsigned int, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaMallocMipmappedArray");
			fprintf(stderr, "original_cudaMallocMipmappedArray:%p\n", original_cudaMallocMipmappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMallocMipmappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMallocMipmappedArray(
		mipmappedArray, 
		desc, 
		extent, 
		numLevels, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}