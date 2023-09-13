#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamQuery)(
cudaStream_t
);
extern "C"
{
	cudaError_t cudaStreamQuery(cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaStreamQuery()\n");
		char* __dlerror;
		if (!original_cudaStreamQuery)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamQuery = (cudaError_t (*)(
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaStreamQuery");
			fprintf(stderr, "original_cudaStreamQuery:%p\n", original_cudaStreamQuery);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamQuery():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamQuery(
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}