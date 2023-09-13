#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetTextureReference)(
const struct textureReference **, 
const void *
);
extern "C"
{
	cudaError_t cudaGetTextureReference(const struct textureReference **texref, const void *symbol) {
		fprintf(stderr, "===============\ncudaGetTextureReference()\n");
		char* __dlerror;
		if (!original_cudaGetTextureReference)
		{
			//fetch the original function addr using dlsym
			original_cudaGetTextureReference = (cudaError_t (*)(
			const struct textureReference **, 
			const void *)
			) dlsym(RTLD_NEXT, "cudaGetTextureReference");
			fprintf(stderr, "original_cudaGetTextureReference:%p\n", original_cudaGetTextureReference);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetTextureReference():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetTextureReference(
		texref, 
		symbol
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}