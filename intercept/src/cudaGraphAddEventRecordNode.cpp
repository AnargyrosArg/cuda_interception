#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddEventRecordNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaGraphAddEventRecordNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaGraphAddEventRecordNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddEventRecordNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddEventRecordNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaGraphAddEventRecordNode");
			fprintf(stderr, "original_cudaGraphAddEventRecordNode:%p\n", original_cudaGraphAddEventRecordNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddEventRecordNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddEventRecordNode(
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