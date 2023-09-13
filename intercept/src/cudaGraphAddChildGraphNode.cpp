#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddChildGraphNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
cudaGraph_t
);
extern "C"
{
	cudaError_t cudaGraphAddChildGraphNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, cudaGraph_t childGraph) {
		fprintf(stderr, "===============\ncudaGraphAddChildGraphNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddChildGraphNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddChildGraphNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			cudaGraph_t)
			) dlsym(RTLD_NEXT, "cudaGraphAddChildGraphNode");
			fprintf(stderr, "original_cudaGraphAddChildGraphNode:%p\n", original_cudaGraphAddChildGraphNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddChildGraphNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddChildGraphNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		childGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}