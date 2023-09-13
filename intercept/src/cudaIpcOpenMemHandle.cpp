#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaIpcOpenMemHandle)(
void **, 
cudaIpcMemHandle_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaIpcOpenMemHandle(void **devPtr, cudaIpcMemHandle_t handle, unsigned int flags) {
		fprintf(stderr, "===============\ncudaIpcOpenMemHandle()\n");
		char* __dlerror;
		if (!original_cudaIpcOpenMemHandle)
		{
			//fetch the original function addr using dlsym
			original_cudaIpcOpenMemHandle = (cudaError_t (*)(
			void **, 
			cudaIpcMemHandle_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaIpcOpenMemHandle");
			fprintf(stderr, "original_cudaIpcOpenMemHandle:%p\n", original_cudaIpcOpenMemHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaIpcOpenMemHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaIpcOpenMemHandle(
		devPtr, 
		handle, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}