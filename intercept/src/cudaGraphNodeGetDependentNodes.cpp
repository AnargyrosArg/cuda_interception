#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphNodeGetDependentNodes)(
cudaGraphNode_t, 
cudaGraphNode_t *, 
size_t *
);
extern "C"
{
	cudaError_t cudaGraphNodeGetDependentNodes(cudaGraphNode_t node, cudaGraphNode_t *pDependentNodes, size_t *pNumDependentNodes) {
		fprintf(stderr, "===============\ncudaGraphNodeGetDependentNodes()\n");
		char* __dlerror;
		if (!original_cudaGraphNodeGetDependentNodes)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphNodeGetDependentNodes = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaGraphNode_t *, 
			size_t *)
			) dlsym(RTLD_NEXT, "cudaGraphNodeGetDependentNodes");
			fprintf(stderr, "original_cudaGraphNodeGetDependentNodes:%p\n", original_cudaGraphNodeGetDependentNodes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphNodeGetDependentNodes():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphNodeGetDependentNodes(
		node, 
		pDependentNodes, 
		pNumDependentNodes
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}