#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphEventWaitNodeSetEvent)(
cudaGraphNode_t, 
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaGraphEventWaitNodeSetEvent(cudaGraphNode_t node, cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaGraphEventWaitNodeSetEvent()\n");
		char* __dlerror;
		if (!original_cudaGraphEventWaitNodeSetEvent)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphEventWaitNodeSetEvent = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaGraphEventWaitNodeSetEvent");
			fprintf(stderr, "original_cudaGraphEventWaitNodeSetEvent:%p\n", original_cudaGraphEventWaitNodeSetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphEventWaitNodeSetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphEventWaitNodeSetEvent(
		node, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}