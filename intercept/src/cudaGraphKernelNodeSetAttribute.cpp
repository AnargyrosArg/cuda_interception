#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphKernelNodeSetAttribute)(
cudaGraphNode_t, 
cudaKernelNodeAttrID, 
const cudaKernelNodeAttrValue *
);
extern "C"
{
	cudaError_t cudaGraphKernelNodeSetAttribute(cudaGraphNode_t hNode, cudaKernelNodeAttrID attr, const cudaKernelNodeAttrValue *value) {
		fprintf(stderr, "===============\ncudaGraphKernelNodeSetAttribute()\n");
		char* __dlerror;
		if (!original_cudaGraphKernelNodeSetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphKernelNodeSetAttribute = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaKernelNodeAttrID, 
			const cudaKernelNodeAttrValue *)
			) dlsym(RTLD_NEXT, "cudaGraphKernelNodeSetAttribute");
			fprintf(stderr, "original_cudaGraphKernelNodeSetAttribute:%p\n", original_cudaGraphKernelNodeSetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphKernelNodeSetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphKernelNodeSetAttribute(
		hNode, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}