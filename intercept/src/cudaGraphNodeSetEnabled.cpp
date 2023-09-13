#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphNodeSetEnabled)(
cudaGraphExec_t, 
cudaGraphNode_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaGraphNodeSetEnabled(cudaGraphExec_t hGraphExec, cudaGraphNode_t hNode, unsigned int isEnabled) {
		fprintf(stderr, "===============\ncudaGraphNodeSetEnabled()\n");
		char* __dlerror;
		if (!original_cudaGraphNodeSetEnabled)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphNodeSetEnabled = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaGraphNodeSetEnabled");
			fprintf(stderr, "original_cudaGraphNodeSetEnabled:%p\n", original_cudaGraphNodeSetEnabled);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphNodeSetEnabled():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphNodeSetEnabled(
		hGraphExec, 
		hNode, 
		isEnabled
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}