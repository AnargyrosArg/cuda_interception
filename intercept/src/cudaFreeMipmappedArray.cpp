#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaFreeMipmappedArray)(
cudaMipmappedArray_t
);
extern "C"
{
	cudaError_t cudaFreeMipmappedArray(cudaMipmappedArray_t mipmappedArray) {
		fprintf(stderr, "===============\ncudaFreeMipmappedArray()\n");
		char* __dlerror;
		if (!original_cudaFreeMipmappedArray)
		{
			//fetch the original function addr using dlsym
			original_cudaFreeMipmappedArray = (cudaError_t (*)(
			cudaMipmappedArray_t)
			) dlsym(RTLD_NEXT, "cudaFreeMipmappedArray");
			fprintf(stderr, "original_cudaFreeMipmappedArray:%p\n", original_cudaFreeMipmappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaFreeMipmappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaFreeMipmappedArray(
		mipmappedArray
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}