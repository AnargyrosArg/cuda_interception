#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaEventRecordWithFlags)(
cudaEvent_t, 
cudaStream_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaEventRecordWithFlags(cudaEvent_t event, cudaStream_t stream, unsigned int flags) {
		fprintf(stderr, "===============\ncudaEventRecordWithFlags()\n");
		char* __dlerror;
		if (!original_cudaEventRecordWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaEventRecordWithFlags = (cudaError_t (*)(
			cudaEvent_t, 
			cudaStream_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaEventRecordWithFlags");
			fprintf(stderr, "original_cudaEventRecordWithFlags:%p\n", original_cudaEventRecordWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaEventRecordWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaEventRecordWithFlags(
		event, 
		stream, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}