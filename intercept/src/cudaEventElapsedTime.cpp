#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaEventElapsedTime)(
float *, 
cudaEvent_t, 
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaEventElapsedTime(float *ms, cudaEvent_t start, cudaEvent_t end) {
		fprintf(stderr, "===============\ncudaEventElapsedTime()\n");
		char* __dlerror;
		if (!original_cudaEventElapsedTime)
		{
			//fetch the original function addr using dlsym
			original_cudaEventElapsedTime = (cudaError_t (*)(
			float *, 
			cudaEvent_t, 
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaEventElapsedTime");
			fprintf(stderr, "original_cudaEventElapsedTime:%p\n", original_cudaEventElapsedTime);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaEventElapsedTime():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaEventElapsedTime(
		ms, 
		start, 
		end
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}