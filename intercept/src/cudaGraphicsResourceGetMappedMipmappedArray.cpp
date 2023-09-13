#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphicsResourceGetMappedMipmappedArray)(
cudaMipmappedArray_t *, 
cudaGraphicsResource_t
);
extern "C"
{
	cudaError_t cudaGraphicsResourceGetMappedMipmappedArray(cudaMipmappedArray_t *mipmappedArray, cudaGraphicsResource_t resource) {
		fprintf(stderr, "===============\ncudaGraphicsResourceGetMappedMipmappedArray()\n");
		char* __dlerror;
		if (!original_cudaGraphicsResourceGetMappedMipmappedArray)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphicsResourceGetMappedMipmappedArray = (cudaError_t (*)(
			cudaMipmappedArray_t *, 
			cudaGraphicsResource_t)
			) dlsym(RTLD_NEXT, "cudaGraphicsResourceGetMappedMipmappedArray");
			fprintf(stderr, "original_cudaGraphicsResourceGetMappedMipmappedArray:%p\n", original_cudaGraphicsResourceGetMappedMipmappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphicsResourceGetMappedMipmappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphicsResourceGetMappedMipmappedArray(
		mipmappedArray, 
		resource
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}