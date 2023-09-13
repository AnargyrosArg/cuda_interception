#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphicsSubResourceGetMappedArray)(
cudaArray_t *, 
cudaGraphicsResource_t, 
unsigned int, 
unsigned int
);
extern "C"
{
	cudaError_t cudaGraphicsSubResourceGetMappedArray(cudaArray_t *array, cudaGraphicsResource_t resource, unsigned int arrayIndex, unsigned int mipLevel) {
		fprintf(stderr, "===============\ncudaGraphicsSubResourceGetMappedArray()\n");
		char* __dlerror;
		if (!original_cudaGraphicsSubResourceGetMappedArray)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphicsSubResourceGetMappedArray = (cudaError_t (*)(
			cudaArray_t *, 
			cudaGraphicsResource_t, 
			unsigned int, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaGraphicsSubResourceGetMappedArray");
			fprintf(stderr, "original_cudaGraphicsSubResourceGetMappedArray:%p\n", original_cudaGraphicsSubResourceGetMappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphicsSubResourceGetMappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphicsSubResourceGetMappedArray(
		array, 
		resource, 
		arrayIndex, 
		mipLevel
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}