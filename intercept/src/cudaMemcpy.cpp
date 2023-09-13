#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy)(
void *, 
const void *, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaMemcpy(void *dst, const void *src, size_t count, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpy()\n");
		char* __dlerror;
		if (!original_cudaMemcpy)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy = (cudaError_t (*)(
			void *, 
			const void *, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpy");
			fprintf(stderr, "original_cudaMemcpy:%p\n", original_cudaMemcpy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy(
		dst, 
		src, 
		count, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}