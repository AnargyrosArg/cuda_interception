#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphEventRecordNodeGetEvent)(
cudaGraphNode_t, 
cudaEvent_t *
);
extern "C"
{
	cudaError_t cudaGraphEventRecordNodeGetEvent(cudaGraphNode_t node, cudaEvent_t *event_out) {
		fprintf(stderr, "===============\ncudaGraphEventRecordNodeGetEvent()\n");
		char* __dlerror;
		if (!original_cudaGraphEventRecordNodeGetEvent)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphEventRecordNodeGetEvent = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaEvent_t *)
			) dlsym(RTLD_NEXT, "cudaGraphEventRecordNodeGetEvent");
			fprintf(stderr, "original_cudaGraphEventRecordNodeGetEvent:%p\n", original_cudaGraphEventRecordNodeGetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphEventRecordNodeGetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphEventRecordNodeGetEvent(
		node, 
		event_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}