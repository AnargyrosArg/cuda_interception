#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMipmappedArrayGetSparseProperties)(
struct cudaArraySparseProperties *, 
cudaMipmappedArray_t
);
extern "C"
{
	cudaError_t cudaMipmappedArrayGetSparseProperties(struct cudaArraySparseProperties *sparseProperties, cudaMipmappedArray_t mipmap) {
		fprintf(stderr, "===============\ncudaMipmappedArrayGetSparseProperties()\n");
		char* __dlerror;
		if (!original_cudaMipmappedArrayGetSparseProperties)
		{
			//fetch the original function addr using dlsym
			original_cudaMipmappedArrayGetSparseProperties = (cudaError_t (*)(
			struct cudaArraySparseProperties *, 
			cudaMipmappedArray_t)
			) dlsym(RTLD_NEXT, "cudaMipmappedArrayGetSparseProperties");
			fprintf(stderr, "original_cudaMipmappedArrayGetSparseProperties:%p\n", original_cudaMipmappedArrayGetSparseProperties);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMipmappedArrayGetSparseProperties():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMipmappedArrayGetSparseProperties(
		sparseProperties, 
		mipmap
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}