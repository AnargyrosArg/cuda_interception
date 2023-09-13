#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamUpdateCaptureDependencies)(
cudaStream_t, 
cudaGraphNode_t *, 
size_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaStreamUpdateCaptureDependencies(cudaStream_t stream, cudaGraphNode_t *dependencies, size_t numDependencies, unsigned int flags) {
		fprintf(stderr, "===============\ncudaStreamUpdateCaptureDependencies()\n");
		char* __dlerror;
		if (!original_cudaStreamUpdateCaptureDependencies)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamUpdateCaptureDependencies = (cudaError_t (*)(
			cudaStream_t, 
			cudaGraphNode_t *, 
			size_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaStreamUpdateCaptureDependencies");
			fprintf(stderr, "original_cudaStreamUpdateCaptureDependencies:%p\n", original_cudaStreamUpdateCaptureDependencies);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamUpdateCaptureDependencies():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamUpdateCaptureDependencies(
		stream, 
		dependencies, 
		numDependencies, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}