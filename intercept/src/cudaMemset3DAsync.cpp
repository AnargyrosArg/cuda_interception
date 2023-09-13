#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemset3DAsync)(
struct cudaPitchedPtr, 
int, 
struct cudaExtent, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemset3DAsync(struct cudaPitchedPtr pitchedDevPtr, int value, struct cudaExtent extent, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemset3DAsync()\n");
		char* __dlerror;
		if (!original_cudaMemset3DAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemset3DAsync = (cudaError_t (*)(
			struct cudaPitchedPtr, 
			int, 
			struct cudaExtent, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemset3DAsync");
			fprintf(stderr, "original_cudaMemset3DAsync:%p\n", original_cudaMemset3DAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemset3DAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemset3DAsync(
		pitchedDevPtr, 
		value, 
		extent, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}