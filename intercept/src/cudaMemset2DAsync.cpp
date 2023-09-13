#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemset2DAsync)(
void *, 
size_t, 
int, 
size_t, 
size_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemset2DAsync(void *devPtr, size_t pitch, int value, size_t width, size_t height, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemset2DAsync()\n");
		char* __dlerror;
		if (!original_cudaMemset2DAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemset2DAsync = (cudaError_t (*)(
			void *, 
			size_t, 
			int, 
			size_t, 
			size_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemset2DAsync");
			fprintf(stderr, "original_cudaMemset2DAsync:%p\n", original_cudaMemset2DAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemset2DAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemset2DAsync(
		devPtr, 
		pitch, 
		value, 
		width, 
		height, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}