#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphicsResourceGetMappedPointer)(
void **, 
size_t *, 
cudaGraphicsResource_t
);
extern "C"
{
	cudaError_t cudaGraphicsResourceGetMappedPointer(void **devPtr, size_t *size, cudaGraphicsResource_t resource) {
		fprintf(stderr, "===============\ncudaGraphicsResourceGetMappedPointer()\n");
		char* __dlerror;
		if (!original_cudaGraphicsResourceGetMappedPointer)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphicsResourceGetMappedPointer = (cudaError_t (*)(
			void **, 
			size_t *, 
			cudaGraphicsResource_t)
			) dlsym(RTLD_NEXT, "cudaGraphicsResourceGetMappedPointer");
			fprintf(stderr, "original_cudaGraphicsResourceGetMappedPointer:%p\n", original_cudaGraphicsResourceGetMappedPointer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphicsResourceGetMappedPointer():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphicsResourceGetMappedPointer(
		devPtr, 
		size, 
		resource
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}