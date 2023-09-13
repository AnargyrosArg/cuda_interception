#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExternalSemaphoresWaitNodeGetParams)(
cudaGraphNode_t, 
struct cudaExternalSemaphoreWaitNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphExternalSemaphoresWaitNodeGetParams(cudaGraphNode_t hNode, struct cudaExternalSemaphoreWaitNodeParams *params_out) {
		fprintf(stderr, "===============\ncudaGraphExternalSemaphoresWaitNodeGetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExternalSemaphoresWaitNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExternalSemaphoresWaitNodeGetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			struct cudaExternalSemaphoreWaitNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphExternalSemaphoresWaitNodeGetParams");
			fprintf(stderr, "original_cudaGraphExternalSemaphoresWaitNodeGetParams:%p\n", original_cudaGraphExternalSemaphoresWaitNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExternalSemaphoresWaitNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExternalSemaphoresWaitNodeGetParams(
		hNode, 
		params_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}