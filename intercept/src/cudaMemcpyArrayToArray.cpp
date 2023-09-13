#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyArrayToArray)(
cudaArray_t, 
size_t, 
size_t, 
cudaArray_const_t, 
size_t, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaMemcpyArrayToArray(cudaArray_t dst, size_t wOffsetDst, size_t hOffsetDst, cudaArray_const_t src, size_t wOffsetSrc, size_t hOffsetSrc, size_t count, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpyArrayToArray()\n");
		char* __dlerror;
		if (!original_cudaMemcpyArrayToArray)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyArrayToArray = (cudaError_t (*)(
			cudaArray_t, 
			size_t, 
			size_t, 
			cudaArray_const_t, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpyArrayToArray");
			fprintf(stderr, "original_cudaMemcpyArrayToArray:%p\n", original_cudaMemcpyArrayToArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyArrayToArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyArrayToArray(
		dst, 
		wOffsetDst, 
		hOffsetDst, 
		src, 
		wOffsetSrc, 
		hOffsetSrc, 
		count, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}