#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecEventRecordNodeSetEvent)(
cudaGraphExec_t, 
cudaGraphNode_t, 
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaGraphExecEventRecordNodeSetEvent(cudaGraphExec_t hGraphExec, cudaGraphNode_t hNode, cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaGraphExecEventRecordNodeSetEvent()\n");
		char* __dlerror;
		if (!original_cudaGraphExecEventRecordNodeSetEvent)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecEventRecordNodeSetEvent = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaGraphExecEventRecordNodeSetEvent");
			fprintf(stderr, "original_cudaGraphExecEventRecordNodeSetEvent:%p\n", original_cudaGraphExecEventRecordNodeSetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecEventRecordNodeSetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecEventRecordNodeSetEvent(
		hGraphExec, 
		hNode, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}