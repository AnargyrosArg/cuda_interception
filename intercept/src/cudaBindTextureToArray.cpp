#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaBindTextureToArray)(
const struct textureReference *, 
cudaArray_const_t, 
const struct cudaChannelFormatDesc *
);
extern "C"
{
	cudaError_t cudaBindTextureToArray(const struct textureReference *texref, cudaArray_const_t array, const struct cudaChannelFormatDesc *desc) {
		fprintf(stderr, "===============\ncudaBindTextureToArray()\n");
		char* __dlerror;
		if (!original_cudaBindTextureToArray)
		{
			//fetch the original function addr using dlsym
			original_cudaBindTextureToArray = (cudaError_t (*)(
			const struct textureReference *, 
			cudaArray_const_t, 
			const struct cudaChannelFormatDesc *)
			) dlsym(RTLD_NEXT, "cudaBindTextureToArray");
			fprintf(stderr, "original_cudaBindTextureToArray:%p\n", original_cudaBindTextureToArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaBindTextureToArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaBindTextureToArray(
		texref, 
		array, 
		desc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}