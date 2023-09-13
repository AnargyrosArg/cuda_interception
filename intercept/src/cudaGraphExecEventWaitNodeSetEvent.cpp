#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecEventWaitNodeSetEvent)(
cudaGraphExec_t, 
cudaGraphNode_t, 
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaGraphExecEventWaitNodeSetEvent(cudaGraphExec_t hGraphExec, cudaGraphNode_t hNode, cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaGraphExecEventWaitNodeSetEvent()\n");
		char* __dlerror;
		if (!original_cudaGraphExecEventWaitNodeSetEvent)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecEventWaitNodeSetEvent = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaGraphExecEventWaitNodeSetEvent");
			fprintf(stderr, "original_cudaGraphExecEventWaitNodeSetEvent:%p\n", original_cudaGraphExecEventWaitNodeSetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecEventWaitNodeSetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecEventWaitNodeSetEvent(
		hGraphExec, 
		hNode, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}