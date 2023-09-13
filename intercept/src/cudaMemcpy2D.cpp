#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy2D)(
void *, 
size_t, 
const void *, 
size_t, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaMemcpy2D(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpy2D()\n");
		char* __dlerror;
		if (!original_cudaMemcpy2D)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy2D = (cudaError_t (*)(
			void *, 
			size_t, 
			const void *, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpy2D");
			fprintf(stderr, "original_cudaMemcpy2D:%p\n", original_cudaMemcpy2D);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy2D():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy2D(
		dst, 
		dpitch, 
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