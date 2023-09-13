#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphicsUnregisterResource)(
cudaGraphicsResource_t
);
extern "C"
{
	cudaError_t cudaGraphicsUnregisterResource(cudaGraphicsResource_t resource) {
		fprintf(stderr, "===============\ncudaGraphicsUnregisterResource()\n");
		char* __dlerror;
		if (!original_cudaGraphicsUnregisterResource)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphicsUnregisterResource = (cudaError_t (*)(
			cudaGraphicsResource_t)
			) dlsym(RTLD_NEXT, "cudaGraphicsUnregisterResource");
			fprintf(stderr, "original_cudaGraphicsUnregisterResource:%p\n", original_cudaGraphicsUnregisterResource);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphicsUnregisterResource():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphicsUnregisterResource(
		resource
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}