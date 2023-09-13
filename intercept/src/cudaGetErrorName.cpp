#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
const char * (*original_cudaGetErrorName)(
cudaError_t
);
extern "C"
{
	const char * cudaGetErrorName(cudaError_t error) {
		fprintf(stderr, "===============\ncudaGetErrorName()\n");
		char* __dlerror;
		if (!original_cudaGetErrorName)
		{
			//fetch the original function addr using dlsym
			original_cudaGetErrorName = (const char * (*)(
			cudaError_t)
			) dlsym(RTLD_NEXT, "cudaGetErrorName");
			fprintf(stderr, "original_cudaGetErrorName:%p\n", original_cudaGetErrorName);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetErrorName():%s\n", __dlerror);
			fflush(stderr);
		}
		const char* retval = original_cudaGetErrorName(
		error
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}