#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy2DArrayToArray)(
cudaArray_t, 
size_t, 
size_t, 
cudaArray_const_t, 
size_t, 
size_t, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaMemcpy2DArrayToArray(cudaArray_t dst, size_t wOffsetDst, size_t hOffsetDst, cudaArray_const_t src, size_t wOffsetSrc, size_t hOffsetSrc, size_t width, size_t height, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpy2DArrayToArray()\n");
		char* __dlerror;
		if (!original_cudaMemcpy2DArrayToArray)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy2DArrayToArray = (cudaError_t (*)(
			cudaArray_t, 
			size_t, 
			size_t, 
			cudaArray_const_t, 
			size_t, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpy2DArrayToArray");
			fprintf(stderr, "original_cudaMemcpy2DArrayToArray:%p\n", original_cudaMemcpy2DArrayToArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy2DArrayToArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy2DArrayToArray(
		dst, 
		wOffsetDst, 
		hOffsetDst, 
		src, 
		wOffsetSrc, 
		hOffsetSrc, 
		width, 
		height, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}