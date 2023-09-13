#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphRemoveDependencies)(
cudaGraph_t, 
const cudaGraphNode_t *, 
const cudaGraphNode_t *, 
size_t
);
extern "C"
{
	cudaError_t cudaGraphRemoveDependencies(cudaGraph_t graph, const cudaGraphNode_t *from, const cudaGraphNode_t *to, size_t numDependencies) {
		fprintf(stderr, "===============\ncudaGraphRemoveDependencies()\n");
		char* __dlerror;
		if (!original_cudaGraphRemoveDependencies)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphRemoveDependencies = (cudaError_t (*)(
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			const cudaGraphNode_t *, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaGraphRemoveDependencies");
			fprintf(stderr, "original_cudaGraphRemoveDependencies:%p\n", original_cudaGraphRemoveDependencies);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphRemoveDependencies():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphRemoveDependencies(
		graph, 
		from, 
		to, 
		numDependencies
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}