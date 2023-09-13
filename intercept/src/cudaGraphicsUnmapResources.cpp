#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphicsUnmapResources)(
int, 
cudaGraphicsResource_t *, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaGraphicsUnmapResources(int count, cudaGraphicsResource_t *resources, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaGraphicsUnmapResources()\n");
		char* __dlerror;
		if (!original_cudaGraphicsUnmapResources)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphicsUnmapResources = (cudaError_t (*)(
			int, 
			cudaGraphicsResource_t *, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaGraphicsUnmapResources");
			fprintf(stderr, "original_cudaGraphicsUnmapResources:%p\n", original_cudaGraphicsUnmapResources);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphicsUnmapResources():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphicsUnmapResources(
		count, 
		resources, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}