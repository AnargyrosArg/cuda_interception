#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamWaitEvent)(
cudaStream_t, 
cudaEvent_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaStreamWaitEvent(cudaStream_t stream, cudaEvent_t event, unsigned int flags) {
		fprintf(stderr, "===============\ncudaStreamWaitEvent()\n");
		char* __dlerror;
		if (!original_cudaStreamWaitEvent)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamWaitEvent = (cudaError_t (*)(
			cudaStream_t, 
			cudaEvent_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaStreamWaitEvent");
			fprintf(stderr, "original_cudaStreamWaitEvent:%p\n", original_cudaStreamWaitEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamWaitEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamWaitEvent(
		stream, 
		event, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}