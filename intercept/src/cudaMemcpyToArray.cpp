#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyToArray)(
cudaArray_t, 
size_t, 
size_t, 
const void *, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaMemcpyToArray(cudaArray_t dst, size_t wOffset, size_t hOffset, const void *src, size_t count, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpyToArray()\n");
		char* __dlerror;
		if (!original_cudaMemcpyToArray)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyToArray = (cudaError_t (*)(
			cudaArray_t, 
			size_t, 
			size_t, 
			const void *, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpyToArray");
			fprintf(stderr, "original_cudaMemcpyToArray:%p\n", original_cudaMemcpyToArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyToArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyToArray(
		dst, 
		wOffset, 
		hOffset, 
		src, 
		count, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}