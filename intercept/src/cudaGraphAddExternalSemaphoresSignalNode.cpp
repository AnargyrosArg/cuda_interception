#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddExternalSemaphoresSignalNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
const struct cudaExternalSemaphoreSignalNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphAddExternalSemaphoresSignalNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, const struct cudaExternalSemaphoreSignalNodeParams *nodeParams) {
		fprintf(stderr, "===============\ncudaGraphAddExternalSemaphoresSignalNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddExternalSemaphoresSignalNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddExternalSemaphoresSignalNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			const struct cudaExternalSemaphoreSignalNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphAddExternalSemaphoresSignalNode");
			fprintf(stderr, "original_cudaGraphAddExternalSemaphoresSignalNode:%p\n", original_cudaGraphAddExternalSemaphoresSignalNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddExternalSemaphoresSignalNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddExternalSemaphoresSignalNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}