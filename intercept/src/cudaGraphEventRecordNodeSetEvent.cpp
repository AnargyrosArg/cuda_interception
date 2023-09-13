#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphEventRecordNodeSetEvent)(
cudaGraphNode_t, 
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaGraphEventRecordNodeSetEvent(cudaGraphNode_t node, cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaGraphEventRecordNodeSetEvent()\n");
		char* __dlerror;
		if (!original_cudaGraphEventRecordNodeSetEvent)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphEventRecordNodeSetEvent = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaGraphEventRecordNodeSetEvent");
			fprintf(stderr, "original_cudaGraphEventRecordNodeSetEvent:%p\n", original_cudaGraphEventRecordNodeSetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphEventRecordNodeSetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphEventRecordNodeSetEvent(
		node, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}