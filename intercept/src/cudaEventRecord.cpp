#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaEventRecord)(
cudaEvent_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaEventRecord(cudaEvent_t event, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaEventRecord()\n");
		char* __dlerror;
		if (!original_cudaEventRecord)
		{
			//fetch the original function addr using dlsym
			original_cudaEventRecord = (cudaError_t (*)(
			cudaEvent_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaEventRecord");
			fprintf(stderr, "original_cudaEventRecord:%p\n", original_cudaEventRecord);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaEventRecord():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaEventRecord(
		event, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}