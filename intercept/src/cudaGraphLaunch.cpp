#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphLaunch)(
cudaGraphExec_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaGraphLaunch(cudaGraphExec_t graphExec, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaGraphLaunch()\n");
		char* __dlerror;
		if (!original_cudaGraphLaunch)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphLaunch = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaGraphLaunch");
			fprintf(stderr, "original_cudaGraphLaunch:%p\n", original_cudaGraphLaunch);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphLaunch():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphLaunch(
		graphExec, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}