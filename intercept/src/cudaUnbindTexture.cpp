#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaUnbindTexture)(
const struct textureReference *
);
extern "C"
{
	cudaError_t cudaUnbindTexture(const struct textureReference *texref) {
		fprintf(stderr, "===============\ncudaUnbindTexture()\n");
		char* __dlerror;
		if (!original_cudaUnbindTexture)
		{
			//fetch the original function addr using dlsym
			original_cudaUnbindTexture = (cudaError_t (*)(
			const struct textureReference *)
			) dlsym(RTLD_NEXT, "cudaUnbindTexture");
			fprintf(stderr, "original_cudaUnbindTexture:%p\n", original_cudaUnbindTexture);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaUnbindTexture():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaUnbindTexture(
		texref
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}