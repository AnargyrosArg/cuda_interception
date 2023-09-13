#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaArrayGetMemoryRequirements)(
struct cudaArrayMemoryRequirements *, 
cudaArray_t, 
int
);
extern "C"
{
	cudaError_t cudaArrayGetMemoryRequirements(struct cudaArrayMemoryRequirements *memoryRequirements, cudaArray_t array, int device) {
		fprintf(stderr, "===============\ncudaArrayGetMemoryRequirements()\n");
		char* __dlerror;
		if (!original_cudaArrayGetMemoryRequirements)
		{
			//fetch the original function addr using dlsym
			original_cudaArrayGetMemoryRequirements = (cudaError_t (*)(
			struct cudaArrayMemoryRequirements *, 
			cudaArray_t, 
			int)
			) dlsym(RTLD_NEXT, "cudaArrayGetMemoryRequirements");
			fprintf(stderr, "original_cudaArrayGetMemoryRequirements:%p\n", original_cudaArrayGetMemoryRequirements);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaArrayGetMemoryRequirements():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaArrayGetMemoryRequirements(
		memoryRequirements, 
		array, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}