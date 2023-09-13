#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaIpcGetMemHandle)(
cudaIpcMemHandle_t *, 
void *
);
extern "C"
{
	cudaError_t cudaIpcGetMemHandle(cudaIpcMemHandle_t *handle, void *devPtr) {
		fprintf(stderr, "===============\ncudaIpcGetMemHandle()\n");
		char* __dlerror;
		if (!original_cudaIpcGetMemHandle)
		{
			//fetch the original function addr using dlsym
			original_cudaIpcGetMemHandle = (cudaError_t (*)(
			cudaIpcMemHandle_t *, 
			void *)
			) dlsym(RTLD_NEXT, "cudaIpcGetMemHandle");
			fprintf(stderr, "original_cudaIpcGetMemHandle:%p\n", original_cudaIpcGetMemHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaIpcGetMemHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaIpcGetMemHandle(
		handle, 
		devPtr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}