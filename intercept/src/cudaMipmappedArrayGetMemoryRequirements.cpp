#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMipmappedArrayGetMemoryRequirements)(
struct cudaArrayMemoryRequirements *, 
cudaMipmappedArray_t, 
int
);
extern "C"
{
	cudaError_t cudaMipmappedArrayGetMemoryRequirements(struct cudaArrayMemoryRequirements *memoryRequirements, cudaMipmappedArray_t mipmap, int device) {
		fprintf(stderr, "===============\ncudaMipmappedArrayGetMemoryRequirements()\n");
		char* __dlerror;
		if (!original_cudaMipmappedArrayGetMemoryRequirements)
		{
			//fetch the original function addr using dlsym
			original_cudaMipmappedArrayGetMemoryRequirements = (cudaError_t (*)(
			struct cudaArrayMemoryRequirements *, 
			cudaMipmappedArray_t, 
			int)
			) dlsym(RTLD_NEXT, "cudaMipmappedArrayGetMemoryRequirements");
			fprintf(stderr, "original_cudaMipmappedArrayGetMemoryRequirements:%p\n", original_cudaMipmappedArrayGetMemoryRequirements);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMipmappedArrayGetMemoryRequirements():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMipmappedArrayGetMemoryRequirements(
		memoryRequirements, 
		mipmap, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}