#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMalloc3DArray)(
cudaArray_t *, 
const struct cudaChannelFormatDesc *, 
struct cudaExtent, 
unsigned int
);
extern "C"
{
	cudaError_t cudaMalloc3DArray(cudaArray_t *array, const struct cudaChannelFormatDesc *desc, struct cudaExtent extent, unsigned int flags) {
		fprintf(stderr, "===============\ncudaMalloc3DArray()\n");
		char* __dlerror;
		if (!original_cudaMalloc3DArray)
		{
			//fetch the original function addr using dlsym
			original_cudaMalloc3DArray = (cudaError_t (*)(
			cudaArray_t *, 
			const struct cudaChannelFormatDesc *, 
			struct cudaExtent, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaMalloc3DArray");
			fprintf(stderr, "original_cudaMalloc3DArray:%p\n", original_cudaMalloc3DArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMalloc3DArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMalloc3DArray(
		array, 
		desc, 
		extent, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}