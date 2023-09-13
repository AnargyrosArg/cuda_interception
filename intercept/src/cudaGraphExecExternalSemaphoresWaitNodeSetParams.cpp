#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecExternalSemaphoresWaitNodeSetParams)(
cudaGraphExec_t, 
cudaGraphNode_t, 
const struct cudaExternalSemaphoreWaitNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphExecExternalSemaphoresWaitNodeSetParams(cudaGraphExec_t hGraphExec, cudaGraphNode_t hNode, const struct cudaExternalSemaphoreWaitNodeParams *nodeParams) {
		fprintf(stderr, "===============\ncudaGraphExecExternalSemaphoresWaitNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExecExternalSemaphoresWaitNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecExternalSemaphoresWaitNodeSetParams = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			const struct cudaExternalSemaphoreWaitNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphExecExternalSemaphoresWaitNodeSetParams");
			fprintf(stderr, "original_cudaGraphExecExternalSemaphoresWaitNodeSetParams:%p\n", original_cudaGraphExecExternalSemaphoresWaitNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecExternalSemaphoresWaitNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecExternalSemaphoresWaitNodeSetParams(
		hGraphExec, 
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}