#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemset3D)(
struct cudaPitchedPtr, 
int, 
struct cudaExtent
);
extern "C"
{
	cudaError_t cudaMemset3D(struct cudaPitchedPtr pitchedDevPtr, int value, struct cudaExtent extent) {
		fprintf(stderr, "===============\ncudaMemset3D()\n");
		char* __dlerror;
		if (!original_cudaMemset3D)
		{
			//fetch the original function addr using dlsym
			original_cudaMemset3D = (cudaError_t (*)(
			struct cudaPitchedPtr, 
			int, 
			struct cudaExtent)
			) dlsym(RTLD_NEXT, "cudaMemset3D");
			fprintf(stderr, "original_cudaMemset3D:%p\n", original_cudaMemset3D);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemset3D():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemset3D(
		pitchedDevPtr, 
		value, 
		extent
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}