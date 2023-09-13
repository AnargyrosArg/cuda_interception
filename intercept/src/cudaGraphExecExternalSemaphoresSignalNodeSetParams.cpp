#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecExternalSemaphoresSignalNodeSetParams)(
cudaGraphExec_t, 
cudaGraphNode_t, 
const struct cudaExternalSemaphoreSignalNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphExecExternalSemaphoresSignalNodeSetParams(cudaGraphExec_t hGraphExec, cudaGraphNode_t hNode, const struct cudaExternalSemaphoreSignalNodeParams *nodeParams) {
		fprintf(stderr, "===============\ncudaGraphExecExternalSemaphoresSignalNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExecExternalSemaphoresSignalNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecExternalSemaphoresSignalNodeSetParams = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			const struct cudaExternalSemaphoreSignalNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphExecExternalSemaphoresSignalNodeSetParams");
			fprintf(stderr, "original_cudaGraphExecExternalSemaphoresSignalNodeSetParams:%p\n", original_cudaGraphExecExternalSemaphoresSignalNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecExternalSemaphoresSignalNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecExternalSemaphoresSignalNodeSetParams(
		hGraphExec, 
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}