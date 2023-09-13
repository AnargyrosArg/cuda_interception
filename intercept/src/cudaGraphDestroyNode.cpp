#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphDestroyNode)(
cudaGraphNode_t
);
extern "C"
{
	cudaError_t cudaGraphDestroyNode(cudaGraphNode_t node) {
		fprintf(stderr, "===============\ncudaGraphDestroyNode()\n");
		char* __dlerror;
		if (!original_cudaGraphDestroyNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphDestroyNode = (cudaError_t (*)(
			cudaGraphNode_t)
			) dlsym(RTLD_NEXT, "cudaGraphDestroyNode");
			fprintf(stderr, "original_cudaGraphDestroyNode:%p\n", original_cudaGraphDestroyNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphDestroyNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphDestroyNode(
		node
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}