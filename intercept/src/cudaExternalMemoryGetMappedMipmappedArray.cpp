#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaExternalMemoryGetMappedMipmappedArray)(
cudaMipmappedArray_t *, 
cudaExternalMemory_t, 
const struct cudaExternalMemoryMipmappedArrayDesc *
);
extern "C"
{
	cudaError_t cudaExternalMemoryGetMappedMipmappedArray(cudaMipmappedArray_t *mipmap, cudaExternalMemory_t extMem, const struct cudaExternalMemoryMipmappedArrayDesc *mipmapDesc) {
		fprintf(stderr, "===============\ncudaExternalMemoryGetMappedMipmappedArray()\n");
		char* __dlerror;
		if (!original_cudaExternalMemoryGetMappedMipmappedArray)
		{
			//fetch the original function addr using dlsym
			original_cudaExternalMemoryGetMappedMipmappedArray = (cudaError_t (*)(
			cudaMipmappedArray_t *, 
			cudaExternalMemory_t, 
			const struct cudaExternalMemoryMipmappedArrayDesc *)
			) dlsym(RTLD_NEXT, "cudaExternalMemoryGetMappedMipmappedArray");
			fprintf(stderr, "original_cudaExternalMemoryGetMappedMipmappedArray:%p\n", original_cudaExternalMemoryGetMappedMipmappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaExternalMemoryGetMappedMipmappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaExternalMemoryGetMappedMipmappedArray(
		mipmap, 
		extMem, 
		mipmapDesc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}