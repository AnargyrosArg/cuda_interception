#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphNodeGetEnabled)(
cudaGraphExec_t, 
cudaGraphNode_t, 
unsigned int *
);
extern "C"
{
	cudaError_t cudaGraphNodeGetEnabled(cudaGraphExec_t hGraphExec, cudaGraphNode_t hNode, unsigned int *isEnabled) {
		fprintf(stderr, "===============\ncudaGraphNodeGetEnabled()\n");
		char* __dlerror;
		if (!original_cudaGraphNodeGetEnabled)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphNodeGetEnabled = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			unsigned int *)
			) dlsym(RTLD_NEXT, "cudaGraphNodeGetEnabled");
			fprintf(stderr, "original_cudaGraphNodeGetEnabled:%p\n", original_cudaGraphNodeGetEnabled);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphNodeGetEnabled():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphNodeGetEnabled(
		hGraphExec, 
		hNode, 
		isEnabled
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}