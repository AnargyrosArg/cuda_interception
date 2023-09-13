#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphEventWaitNodeGetEvent)(
cudaGraphNode_t, 
cudaEvent_t *
);
extern "C"
{
	cudaError_t cudaGraphEventWaitNodeGetEvent(cudaGraphNode_t node, cudaEvent_t *event_out) {
		fprintf(stderr, "===============\ncudaGraphEventWaitNodeGetEvent()\n");
		char* __dlerror;
		if (!original_cudaGraphEventWaitNodeGetEvent)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphEventWaitNodeGetEvent = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaEvent_t *)
			) dlsym(RTLD_NEXT, "cudaGraphEventWaitNodeGetEvent");
			fprintf(stderr, "original_cudaGraphEventWaitNodeGetEvent:%p\n", original_cudaGraphEventWaitNodeGetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphEventWaitNodeGetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphEventWaitNodeGetEvent(
		node, 
		event_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}