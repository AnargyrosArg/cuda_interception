#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamAttachMemAsync)(
cudaStream_t, 
void *, 
size_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaStreamAttachMemAsync(cudaStream_t stream, void *devPtr, size_t length, unsigned int flags) {
		fprintf(stderr, "===============\ncudaStreamAttachMemAsync()\n");
		char* __dlerror;
		if (!original_cudaStreamAttachMemAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamAttachMemAsync = (cudaError_t (*)(
			cudaStream_t, 
			void *, 
			size_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaStreamAttachMemAsync");
			fprintf(stderr, "original_cudaStreamAttachMemAsync:%p\n", original_cudaStreamAttachMemAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamAttachMemAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamAttachMemAsync(
		stream, 
		devPtr, 
		length, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}