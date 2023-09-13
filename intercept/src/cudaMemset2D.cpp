#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemset2D)(
void *, 
size_t, 
int, 
size_t, 
size_t
);
extern "C"
{
	cudaError_t cudaMemset2D(void *devPtr, size_t pitch, int value, size_t width, size_t height) {
		fprintf(stderr, "===============\ncudaMemset2D()\n");
		char* __dlerror;
		if (!original_cudaMemset2D)
		{
			//fetch the original function addr using dlsym
			original_cudaMemset2D = (cudaError_t (*)(
			void *, 
			size_t, 
			int, 
			size_t, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMemset2D");
			fprintf(stderr, "original_cudaMemset2D:%p\n", original_cudaMemset2D);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemset2D():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemset2D(
		devPtr, 
		pitch, 
		value, 
		width, 
		height
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}