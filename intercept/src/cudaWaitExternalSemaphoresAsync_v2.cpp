#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaWaitExternalSemaphoresAsync_v2)(
const cudaExternalSemaphore_t *, 
const struct cudaExternalSemaphoreWaitParams *, 
unsigned int, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaWaitExternalSemaphoresAsync_v2(const cudaExternalSemaphore_t *extSemArray, const struct cudaExternalSemaphoreWaitParams *paramsArray, unsigned int numExtSems, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaWaitExternalSemaphoresAsync_v2()\n");
		char* __dlerror;
		if (!original_cudaWaitExternalSemaphoresAsync_v2)
		{
			//fetch the original function addr using dlsym
			original_cudaWaitExternalSemaphoresAsync_v2 = (cudaError_t (*)(
			const cudaExternalSemaphore_t *, 
			const struct cudaExternalSemaphoreWaitParams *, 
			unsigned int, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaWaitExternalSemaphoresAsync_v2");
			fprintf(stderr, "original_cudaWaitExternalSemaphoresAsync_v2:%p\n", original_cudaWaitExternalSemaphoresAsync_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaWaitExternalSemaphoresAsync_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaWaitExternalSemaphoresAsync_v2(
		extSemArray, 
		paramsArray, 
		numExtSems, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}