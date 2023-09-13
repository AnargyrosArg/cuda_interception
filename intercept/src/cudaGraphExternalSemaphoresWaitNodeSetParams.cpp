#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExternalSemaphoresWaitNodeSetParams)(
cudaGraphNode_t, 
const struct cudaExternalSemaphoreWaitNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphExternalSemaphoresWaitNodeSetParams(cudaGraphNode_t hNode, const struct cudaExternalSemaphoreWaitNodeParams *nodeParams) {
		fprintf(stderr, "===============\ncudaGraphExternalSemaphoresWaitNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExternalSemaphoresWaitNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExternalSemaphoresWaitNodeSetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			const struct cudaExternalSemaphoreWaitNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphExternalSemaphoresWaitNodeSetParams");
			fprintf(stderr, "original_cudaGraphExternalSemaphoresWaitNodeSetParams:%p\n", original_cudaGraphExternalSemaphoresWaitNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExternalSemaphoresWaitNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExternalSemaphoresWaitNodeSetParams(
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}