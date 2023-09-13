#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaIpcOpenEventHandle)(
cudaEvent_t *, 
cudaIpcEventHandle_t
);
extern "C"
{
	cudaError_t cudaIpcOpenEventHandle(cudaEvent_t *event, cudaIpcEventHandle_t handle) {
		fprintf(stderr, "===============\ncudaIpcOpenEventHandle()\n");
		char* __dlerror;
		if (!original_cudaIpcOpenEventHandle)
		{
			//fetch the original function addr using dlsym
			original_cudaIpcOpenEventHandle = (cudaError_t (*)(
			cudaEvent_t *, 
			cudaIpcEventHandle_t)
			) dlsym(RTLD_NEXT, "cudaIpcOpenEventHandle");
			fprintf(stderr, "original_cudaIpcOpenEventHandle:%p\n", original_cudaIpcOpenEventHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaIpcOpenEventHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaIpcOpenEventHandle(
		event, 
		handle
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}