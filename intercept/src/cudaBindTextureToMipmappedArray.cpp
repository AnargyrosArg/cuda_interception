#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaBindTextureToMipmappedArray)(
const struct textureReference *, 
cudaMipmappedArray_const_t, 
const struct cudaChannelFormatDesc *
);
extern "C"
{
	cudaError_t cudaBindTextureToMipmappedArray(const struct textureReference *texref, cudaMipmappedArray_const_t mipmappedArray, const struct cudaChannelFormatDesc *desc) {
		fprintf(stderr, "===============\ncudaBindTextureToMipmappedArray()\n");
		char* __dlerror;
		if (!original_cudaBindTextureToMipmappedArray)
		{
			//fetch the original function addr using dlsym
			original_cudaBindTextureToMipmappedArray = (cudaError_t (*)(
			const struct textureReference *, 
			cudaMipmappedArray_const_t, 
			const struct cudaChannelFormatDesc *)
			) dlsym(RTLD_NEXT, "cudaBindTextureToMipmappedArray");
			fprintf(stderr, "original_cudaBindTextureToMipmappedArray:%p\n", original_cudaBindTextureToMipmappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaBindTextureToMipmappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaBindTextureToMipmappedArray(
		texref, 
		mipmappedArray, 
		desc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}