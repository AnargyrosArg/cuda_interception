#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphMemcpyNodeSetParams1D)(
cudaGraphNode_t, 
void *, 
const void *, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaGraphMemcpyNodeSetParams1D(cudaGraphNode_t node, void *dst, const void *src, size_t count, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaGraphMemcpyNodeSetParams1D()\n");
		char* __dlerror;
		if (!original_cudaGraphMemcpyNodeSetParams1D)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphMemcpyNodeSetParams1D = (cudaError_t (*)(
			cudaGraphNode_t, 
			void *, 
			const void *, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaGraphMemcpyNodeSetParams1D");
			fprintf(stderr, "original_cudaGraphMemcpyNodeSetParams1D:%p\n", original_cudaGraphMemcpyNodeSetParams1D);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphMemcpyNodeSetParams1D():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphMemcpyNodeSetParams1D(
		node, 
		dst, 
		src, 
		count, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}