#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamCopyAttributes)(
cudaStream_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaStreamCopyAttributes(cudaStream_t dst, cudaStream_t src) {
		fprintf(stderr, "===============\ncudaStreamCopyAttributes()\n");
		char* __dlerror;
		if (!original_cudaStreamCopyAttributes)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamCopyAttributes = (cudaError_t (*)(
			cudaStream_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaStreamCopyAttributes");
			fprintf(stderr, "original_cudaStreamCopyAttributes:%p\n", original_cudaStreamCopyAttributes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamCopyAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamCopyAttributes(
		dst, 
		src
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}