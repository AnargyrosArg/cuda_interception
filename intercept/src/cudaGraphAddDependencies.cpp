#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddDependencies)(
cudaGraph_t, 
const cudaGraphNode_t *, 
const cudaGraphNode_t *, 
size_t
);
extern "C"
{
	cudaError_t cudaGraphAddDependencies(cudaGraph_t graph, const cudaGraphNode_t *from, const cudaGraphNode_t *to, size_t numDependencies) {
		fprintf(stderr, "===============\ncudaGraphAddDependencies()\n");
		char* __dlerror;
		if (!original_cudaGraphAddDependencies)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddDependencies = (cudaError_t (*)(
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			const cudaGraphNode_t *, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaGraphAddDependencies");
			fprintf(stderr, "original_cudaGraphAddDependencies:%p\n", original_cudaGraphAddDependencies);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddDependencies():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddDependencies(
		graph, 
		from, 
		to, 
		numDependencies
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}