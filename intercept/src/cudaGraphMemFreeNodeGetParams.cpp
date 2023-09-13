#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphMemFreeNodeGetParams)(
cudaGraphNode_t, 
void *
);
extern "C"
{
	cudaError_t cudaGraphMemFreeNodeGetParams(cudaGraphNode_t node, void *dptr_out) {
		fprintf(stderr, "===============\ncudaGraphMemFreeNodeGetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphMemFreeNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphMemFreeNodeGetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			void *)
			) dlsym(RTLD_NEXT, "cudaGraphMemFreeNodeGetParams");
			fprintf(stderr, "original_cudaGraphMemFreeNodeGetParams:%p\n", original_cudaGraphMemFreeNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphMemFreeNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphMemFreeNodeGetParams(
		node, 
		dptr_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}