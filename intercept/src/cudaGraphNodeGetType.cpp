#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphNodeGetType)(
cudaGraphNode_t, 
enum cudaGraphNodeType *
);
extern "C"
{
	cudaError_t cudaGraphNodeGetType(cudaGraphNode_t node, enum cudaGraphNodeType *pType) {
		fprintf(stderr, "===============\ncudaGraphNodeGetType()\n");
		char* __dlerror;
		if (!original_cudaGraphNodeGetType)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphNodeGetType = (cudaError_t (*)(
			cudaGraphNode_t, 
			enum cudaGraphNodeType *)
			) dlsym(RTLD_NEXT, "cudaGraphNodeGetType");
			fprintf(stderr, "original_cudaGraphNodeGetType:%p\n", original_cudaGraphNodeGetType);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphNodeGetType():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphNodeGetType(
		node, 
		pType
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}