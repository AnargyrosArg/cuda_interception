#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetStreamPriorityRange)(
int *, 
int *
);
extern "C"
{
	cudaError_t cudaDeviceGetStreamPriorityRange(int *leastPriority, int *greatestPriority) {
		fprintf(stderr, "===============\ncudaDeviceGetStreamPriorityRange()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetStreamPriorityRange)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetStreamPriorityRange = (cudaError_t (*)(
			int *, 
			int *)
			) dlsym(RTLD_NEXT, "cudaDeviceGetStreamPriorityRange");
			fprintf(stderr, "original_cudaDeviceGetStreamPriorityRange:%p\n", original_cudaDeviceGetStreamPriorityRange);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetStreamPriorityRange():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetStreamPriorityRange(
		leastPriority, 
		greatestPriority
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}