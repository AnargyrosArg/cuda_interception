#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddEmptyNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t
);
extern "C"
{
	cudaError_t cudaGraphAddEmptyNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies) {
		fprintf(stderr, "===============\ncudaGraphAddEmptyNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddEmptyNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddEmptyNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaGraphAddEmptyNode");
			fprintf(stderr, "original_cudaGraphAddEmptyNode:%p\n", original_cudaGraphAddEmptyNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddEmptyNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddEmptyNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}