#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaIpcGetEventHandle)(
cudaIpcEventHandle_t *, 
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaIpcGetEventHandle(cudaIpcEventHandle_t *handle, cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaIpcGetEventHandle()\n");
		char* __dlerror;
		if (!original_cudaIpcGetEventHandle)
		{
			//fetch the original function addr using dlsym
			original_cudaIpcGetEventHandle = (cudaError_t (*)(
			cudaIpcEventHandle_t *, 
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaIpcGetEventHandle");
			fprintf(stderr, "original_cudaIpcGetEventHandle:%p\n", original_cudaIpcGetEventHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaIpcGetEventHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaIpcGetEventHandle(
		handle, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}