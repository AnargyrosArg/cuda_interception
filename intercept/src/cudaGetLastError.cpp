#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetLastError)(
);
extern "C"
{
	cudaError_t cudaGetLastError() {
		fprintf(stderr, "===============\ncudaGetLastError()\n");
		char* __dlerror;
		if (!original_cudaGetLastError)
		{
			//fetch the original function addr using dlsym
			original_cudaGetLastError = (cudaError_t (*)(
			void)
			) dlsym(RTLD_NEXT, "cudaGetLastError");
			fprintf(stderr, "original_cudaGetLastError:%p\n", original_cudaGetLastError);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetLastError():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetLastError(
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}