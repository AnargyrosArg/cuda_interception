#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphMemAllocNodeGetParams)(
cudaGraphNode_t, 
struct cudaMemAllocNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphMemAllocNodeGetParams(cudaGraphNode_t node, struct cudaMemAllocNodeParams *params_out) {
		fprintf(stderr, "===============\ncudaGraphMemAllocNodeGetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphMemAllocNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphMemAllocNodeGetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			struct cudaMemAllocNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphMemAllocNodeGetParams");
			fprintf(stderr, "original_cudaGraphMemAllocNodeGetParams:%p\n", original_cudaGraphMemAllocNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphMemAllocNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphMemAllocNodeGetParams(
		node, 
		params_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}