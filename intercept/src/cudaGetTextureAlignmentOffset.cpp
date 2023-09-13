#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetTextureAlignmentOffset)(
size_t *, 
const struct textureReference *
);
extern "C"
{
	cudaError_t cudaGetTextureAlignmentOffset(size_t *offset, const struct textureReference *texref) {
		fprintf(stderr, "===============\ncudaGetTextureAlignmentOffset()\n");
		char* __dlerror;
		if (!original_cudaGetTextureAlignmentOffset)
		{
			//fetch the original function addr using dlsym
			original_cudaGetTextureAlignmentOffset = (cudaError_t (*)(
			size_t *, 
			const struct textureReference *)
			) dlsym(RTLD_NEXT, "cudaGetTextureAlignmentOffset");
			fprintf(stderr, "original_cudaGetTextureAlignmentOffset:%p\n", original_cudaGetTextureAlignmentOffset);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetTextureAlignmentOffset():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetTextureAlignmentOffset(
		offset, 
		texref
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}