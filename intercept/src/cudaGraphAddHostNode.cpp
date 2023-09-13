#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddHostNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
const struct cudaHostNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphAddHostNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, const struct cudaHostNodeParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphAddHostNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddHostNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddHostNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			const struct cudaHostNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphAddHostNode");
			fprintf(stderr, "original_cudaGraphAddHostNode:%p\n", original_cudaGraphAddHostNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddHostNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddHostNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}