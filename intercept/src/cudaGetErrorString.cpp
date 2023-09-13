#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
const char * (*original_cudaGetErrorString)(
cudaError_t
);
extern "C"
{
	const char * cudaGetErrorString(cudaError_t error) {
		fprintf(stderr, "===============\ncudaGetErrorString()\n");
		char* __dlerror;
		if (!original_cudaGetErrorString)
		{
			//fetch the original function addr using dlsym
			original_cudaGetErrorString = (const char * (*)(
			cudaError_t)
			) dlsym(RTLD_NEXT, "cudaGetErrorString");
			fprintf(stderr, "original_cudaGetErrorString:%p\n", original_cudaGetErrorString);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetErrorString():%s\n", __dlerror);
			fflush(stderr);
		}
		const char* retval = original_cudaGetErrorString(
		error
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}