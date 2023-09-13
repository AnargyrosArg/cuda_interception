#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphicsMapResources)(
int, 
cudaGraphicsResource_t *, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaGraphicsMapResources(int count, cudaGraphicsResource_t *resources, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaGraphicsMapResources()\n");
		char* __dlerror;
		if (!original_cudaGraphicsMapResources)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphicsMapResources = (cudaError_t (*)(
			int, 
			cudaGraphicsResource_t *, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaGraphicsMapResources");
			fprintf(stderr, "original_cudaGraphicsMapResources:%p\n", original_cudaGraphicsMapResources);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphicsMapResources():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphicsMapResources(
		count, 
		resources, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}