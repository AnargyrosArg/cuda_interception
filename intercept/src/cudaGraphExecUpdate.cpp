#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecUpdate)(
cudaGraphExec_t, 
cudaGraph_t, 
cudaGraphNode_t *, 
enum cudaGraphExecUpdateResult *
);
extern "C"
{
	cudaError_t cudaGraphExecUpdate(cudaGraphExec_t hGraphExec, cudaGraph_t hGraph, cudaGraphNode_t *hErrorNode_out, enum cudaGraphExecUpdateResult *updateResult_out) {
		fprintf(stderr, "===============\ncudaGraphExecUpdate()\n");
		char* __dlerror;
		if (!original_cudaGraphExecUpdate)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecUpdate = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraph_t, 
			cudaGraphNode_t *, 
			enum cudaGraphExecUpdateResult *)
			) dlsym(RTLD_NEXT, "cudaGraphExecUpdate");
			fprintf(stderr, "original_cudaGraphExecUpdate:%p\n", original_cudaGraphExecUpdate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecUpdate():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecUpdate(
		hGraphExec, 
		hGraph, 
		hErrorNode_out, 
		updateResult_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}