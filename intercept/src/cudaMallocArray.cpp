#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMallocArray)(
cudaArray_t *, 
const struct cudaChannelFormatDesc *, 
size_t, 
size_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaMallocArray(cudaArray_t *array, const struct cudaChannelFormatDesc *desc, size_t width, size_t height, unsigned int flags) {
		fprintf(stderr, "===============\ncudaMallocArray()\n");
		char* __dlerror;
		if (!original_cudaMallocArray)
		{
			//fetch the original function addr using dlsym
			original_cudaMallocArray = (cudaError_t (*)(
			cudaArray_t *, 
			const struct cudaChannelFormatDesc *, 
			size_t, 
			size_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaMallocArray");
			fprintf(stderr, "original_cudaMallocArray:%p\n", original_cudaMallocArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMallocArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMallocArray(
		array, 
		desc, 
		width, 
		height, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}