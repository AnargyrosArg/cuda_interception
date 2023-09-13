#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy2DAsync)(
void *, 
size_t, 
const void *, 
size_t, 
size_t, 
size_t, 
enum cudaMemcpyKind, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpy2DAsync(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height, enum cudaMemcpyKind kind, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpy2DAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpy2DAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy2DAsync = (cudaError_t (*)(
			void *, 
			size_t, 
			const void *, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpy2DAsync");
			fprintf(stderr, "original_cudaMemcpy2DAsync:%p\n", original_cudaMemcpy2DAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy2DAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy2DAsync(
		dst, 
		dpitch, 
		src, 
		spitch, 
		width, 
		height, 
		kind, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}