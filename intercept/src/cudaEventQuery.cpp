#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaEventQuery)(
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaEventQuery(cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaEventQuery()\n");
		char* __dlerror;
		if (!original_cudaEventQuery)
		{
			//fetch the original function addr using dlsym
			original_cudaEventQuery = (cudaError_t (*)(
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaEventQuery");
			fprintf(stderr, "original_cudaEventQuery:%p\n", original_cudaEventQuery);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaEventQuery():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaEventQuery(
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}