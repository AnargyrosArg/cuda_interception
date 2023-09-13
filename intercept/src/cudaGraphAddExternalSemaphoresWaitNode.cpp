#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddExternalSemaphoresWaitNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
const struct cudaExternalSemaphoreWaitNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphAddExternalSemaphoresWaitNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, const struct cudaExternalSemaphoreWaitNodeParams *nodeParams) {
		fprintf(stderr, "===============\ncudaGraphAddExternalSemaphoresWaitNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddExternalSemaphoresWaitNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddExternalSemaphoresWaitNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			const struct cudaExternalSemaphoreWaitNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphAddExternalSemaphoresWaitNode");
			fprintf(stderr, "original_cudaGraphAddExternalSemaphoresWaitNode:%p\n", original_cudaGraphAddExternalSemaphoresWaitNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddExternalSemaphoresWaitNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddExternalSemaphoresWaitNode(
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