#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExternalSemaphoresSignalNodeGetParams)(
cudaGraphNode_t, 
struct cudaExternalSemaphoreSignalNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphExternalSemaphoresSignalNodeGetParams(cudaGraphNode_t hNode, struct cudaExternalSemaphoreSignalNodeParams *params_out) {
		fprintf(stderr, "===============\ncudaGraphExternalSemaphoresSignalNodeGetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExternalSemaphoresSignalNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExternalSemaphoresSignalNodeGetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			struct cudaExternalSemaphoreSignalNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphExternalSemaphoresSignalNodeGetParams");
			fprintf(stderr, "original_cudaGraphExternalSemaphoresSignalNodeGetParams:%p\n", original_cudaGraphExternalSemaphoresSignalNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExternalSemaphoresSignalNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExternalSemaphoresSignalNodeGetParams(
		hNode, 
		params_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}