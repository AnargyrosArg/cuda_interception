#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddEventWaitNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaGraphAddEventWaitNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaGraphAddEventWaitNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddEventWaitNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddEventWaitNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaGraphAddEventWaitNode");
			fprintf(stderr, "original_cudaGraphAddEventWaitNode:%p\n", original_cudaGraphAddEventWaitNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddEventWaitNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddEventWaitNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}