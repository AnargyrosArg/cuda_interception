#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExternalSemaphoresSignalNodeSetParams)(
cudaGraphNode_t, 
const struct cudaExternalSemaphoreSignalNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphExternalSemaphoresSignalNodeSetParams(cudaGraphNode_t hNode, const struct cudaExternalSemaphoreSignalNodeParams *nodeParams) {
		fprintf(stderr, "===============\ncudaGraphExternalSemaphoresSignalNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExternalSemaphoresSignalNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExternalSemaphoresSignalNodeSetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			const struct cudaExternalSemaphoreSignalNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphExternalSemaphoresSignalNodeSetParams");
			fprintf(stderr, "original_cudaGraphExternalSemaphoresSignalNodeSetParams:%p\n", original_cudaGraphExternalSemaphoresSignalNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExternalSemaphoresSignalNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExternalSemaphoresSignalNodeSetParams(
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}