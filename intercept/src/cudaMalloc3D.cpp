#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMalloc3D)(
struct cudaPitchedPtr *, 
struct cudaExtent
);
extern "C"
{
	cudaError_t cudaMalloc3D(struct cudaPitchedPtr *pitchedDevPtr, struct cudaExtent extent) {
		fprintf(stderr, "===============\ncudaMalloc3D()\n");
		char* __dlerror;
		if (!original_cudaMalloc3D)
		{
			//fetch the original function addr using dlsym
			original_cudaMalloc3D = (cudaError_t (*)(
			struct cudaPitchedPtr *, 
			struct cudaExtent)
			) dlsym(RTLD_NEXT, "cudaMalloc3D");
			fprintf(stderr, "original_cudaMalloc3D:%p\n", original_cudaMalloc3D);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMalloc3D():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMalloc3D(
		pitchedDevPtr, 
		extent
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}