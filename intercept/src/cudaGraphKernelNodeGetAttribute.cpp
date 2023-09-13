#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphKernelNodeGetAttribute)(
cudaGraphNode_t, 
cudaKernelNodeAttrID, 
cudaKernelNodeAttrValue *
);
extern "C"
{
	cudaError_t cudaGraphKernelNodeGetAttribute(cudaGraphNode_t hNode, cudaKernelNodeAttrID attr, cudaKernelNodeAttrValue *value_out) {
		fprintf(stderr, "===============\ncudaGraphKernelNodeGetAttribute()\n");
		char* __dlerror;
		if (!original_cudaGraphKernelNodeGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphKernelNodeGetAttribute = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaKernelNodeAttrID, 
			cudaKernelNodeAttrValue *)
			) dlsym(RTLD_NEXT, "cudaGraphKernelNodeGetAttribute");
			fprintf(stderr, "original_cudaGraphKernelNodeGetAttribute:%p\n", original_cudaGraphKernelNodeGetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphKernelNodeGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphKernelNodeGetAttribute(
		hNode, 
		attr, 
		value_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}