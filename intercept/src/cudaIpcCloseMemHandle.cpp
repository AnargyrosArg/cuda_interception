#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaIpcCloseMemHandle)(
void *
);
extern "C"
{
	cudaError_t cudaIpcCloseMemHandle(void *devPtr) {
		fprintf(stderr, "===============\ncudaIpcCloseMemHandle()\n");
		char* __dlerror;
		if (!original_cudaIpcCloseMemHandle)
		{
			//fetch the original function addr using dlsym
			original_cudaIpcCloseMemHandle = (cudaError_t (*)(
			void *)
			) dlsym(RTLD_NEXT, "cudaIpcCloseMemHandle");
			fprintf(stderr, "original_cudaIpcCloseMemHandle:%p\n", original_cudaIpcCloseMemHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaIpcCloseMemHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaIpcCloseMemHandle(
		devPtr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}