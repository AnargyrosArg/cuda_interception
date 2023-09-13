#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy2DToArray)(
cudaArray_t, 
size_t, 
size_t, 
const void *, 
size_t, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaMemcpy2DToArray(cudaArray_t dst, size_t wOffset, size_t hOffset, const void *src, size_t spitch, size_t width, size_t height, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpy2DToArray()\n");
		char* __dlerror;
		if (!original_cudaMemcpy2DToArray)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy2DToArray = (cudaError_t (*)(
			cudaArray_t, 
			size_t, 
			size_t, 
			const void *, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpy2DToArray");
			fprintf(stderr, "original_cudaMemcpy2DToArray:%p\n", original_cudaMemcpy2DToArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy2DToArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy2DToArray(
		dst, 
		wOffset, 
		hOffset, 
		src, 
		spitch, 
		width, 
		height, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}