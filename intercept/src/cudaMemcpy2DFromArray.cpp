#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy2DFromArray)(
void *, 
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
	cudaError_t cudaMemcpy2DFromArray(void *dst, size_t dpitch, cudaArray_const_t src, size_t wOffset, size_t hOffset, size_t width, size_t height, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpy2DFromArray()\n");
		char* __dlerror;
		if (!original_cudaMemcpy2DFromArray)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy2DFromArray = (cudaError_t (*)(
			void *, 
			size_t, 
			cudaArray_const_t, 
			size_t, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpy2DFromArray");
			fprintf(stderr, "original_cudaMemcpy2DFromArray:%p\n", original_cudaMemcpy2DFromArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy2DFromArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy2DFromArray(
		dst, 
		dpitch, 
		src, 
		wOffset, 
		hOffset, 
		width, 
		height, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}