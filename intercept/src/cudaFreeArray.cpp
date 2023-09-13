#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaFreeArray)(
cudaArray_t
);
extern "C"
{
	cudaError_t cudaFreeArray(cudaArray_t array) {
		fprintf(stderr, "===============\ncudaFreeArray()\n");
		char* __dlerror;
		if (!original_cudaFreeArray)
		{
			//fetch the original function addr using dlsym
			original_cudaFreeArray = (cudaError_t (*)(
			cudaArray_t)
			) dlsym(RTLD_NEXT, "cudaFreeArray");
			fprintf(stderr, "original_cudaFreeArray:%p\n", original_cudaFreeArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaFreeArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaFreeArray(
		array
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}