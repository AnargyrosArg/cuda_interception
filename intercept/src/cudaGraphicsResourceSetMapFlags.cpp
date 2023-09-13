#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphicsResourceSetMapFlags)(
cudaGraphicsResource_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaGraphicsResourceSetMapFlags(cudaGraphicsResource_t resource, unsigned int flags) {
		fprintf(stderr, "===============\ncudaGraphicsResourceSetMapFlags()\n");
		char* __dlerror;
		if (!original_cudaGraphicsResourceSetMapFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphicsResourceSetMapFlags = (cudaError_t (*)(
			cudaGraphicsResource_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaGraphicsResourceSetMapFlags");
			fprintf(stderr, "original_cudaGraphicsResourceSetMapFlags:%p\n", original_cudaGraphicsResourceSetMapFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphicsResourceSetMapFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphicsResourceSetMapFlags(
		resource, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}