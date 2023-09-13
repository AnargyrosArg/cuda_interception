#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaArrayGetSparseProperties)(
struct cudaArraySparseProperties *, 
cudaArray_t
);
extern "C"
{
	cudaError_t cudaArrayGetSparseProperties(struct cudaArraySparseProperties *sparseProperties, cudaArray_t array) {
		fprintf(stderr, "===============\ncudaArrayGetSparseProperties()\n");
		char* __dlerror;
		if (!original_cudaArrayGetSparseProperties)
		{
			//fetch the original function addr using dlsym
			original_cudaArrayGetSparseProperties = (cudaError_t (*)(
			struct cudaArraySparseProperties *, 
			cudaArray_t)
			) dlsym(RTLD_NEXT, "cudaArrayGetSparseProperties");
			fprintf(stderr, "original_cudaArrayGetSparseProperties:%p\n", original_cudaArrayGetSparseProperties);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaArrayGetSparseProperties():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaArrayGetSparseProperties(
		sparseProperties, 
		array
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}