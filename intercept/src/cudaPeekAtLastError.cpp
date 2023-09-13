#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaPeekAtLastError)(
);
extern "C"
{
	cudaError_t cudaPeekAtLastError() {
		fprintf(stderr, "===============\ncudaPeekAtLastError()\n");
		char* __dlerror;
		if (!original_cudaPeekAtLastError)
		{
			//fetch the original function addr using dlsym
			original_cudaPeekAtLastError = (cudaError_t (*)(
			void)
			) dlsym(RTLD_NEXT, "cudaPeekAtLastError");
			fprintf(stderr, "original_cudaPeekAtLastError:%p\n", original_cudaPeekAtLastError);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaPeekAtLastError():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaPeekAtLastError(
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}