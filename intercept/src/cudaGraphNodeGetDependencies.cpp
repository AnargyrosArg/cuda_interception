#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphNodeGetDependencies)(
cudaGraphNode_t, 
cudaGraphNode_t *, 
size_t *
);
extern "C"
{
	cudaError_t cudaGraphNodeGetDependencies(cudaGraphNode_t node, cudaGraphNode_t *pDependencies, size_t *pNumDependencies) {
		fprintf(stderr, "===============\ncudaGraphNodeGetDependencies()\n");
		char* __dlerror;
		if (!original_cudaGraphNodeGetDependencies)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphNodeGetDependencies = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaGraphNode_t *, 
			size_t *)
			) dlsym(RTLD_NEXT, "cudaGraphNodeGetDependencies");
			fprintf(stderr, "original_cudaGraphNodeGetDependencies:%p\n", original_cudaGraphNodeGetDependencies);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphNodeGetDependencies():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphNodeGetDependencies(
		node, 
		pDependencies, 
		pNumDependencies
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}