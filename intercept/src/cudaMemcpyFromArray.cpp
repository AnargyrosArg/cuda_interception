#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyFromArray)(
void *, 
cudaArray_const_t, 
size_t, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaMemcpyFromArray(void *dst, cudaArray_const_t src, size_t wOffset, size_t hOffset, size_t count, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpyFromArray()\n");
		char* __dlerror;
		if (!original_cudaMemcpyFromArray)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyFromArray = (cudaError_t (*)(
			void *, 
			cudaArray_const_t, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpyFromArray");
			fprintf(stderr, "original_cudaMemcpyFromArray:%p\n", original_cudaMemcpyFromArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyFromArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyFromArray(
		dst, 
		src, 
		wOffset, 
		hOffset, 
		count, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}