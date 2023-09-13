#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetTextureObjectResourceDesc)(
struct cudaResourceDesc *, 
cudaTextureObject_t
);
extern "C"
{
	cudaError_t cudaGetTextureObjectResourceDesc(struct cudaResourceDesc *pResDesc, cudaTextureObject_t texObject) {
		fprintf(stderr, "===============\ncudaGetTextureObjectResourceDesc()\n");
		char* __dlerror;
		if (!original_cudaGetTextureObjectResourceDesc)
		{
			//fetch the original function addr using dlsym
			original_cudaGetTextureObjectResourceDesc = (cudaError_t (*)(
			struct cudaResourceDesc *, 
			cudaTextureObject_t)
			) dlsym(RTLD_NEXT, "cudaGetTextureObjectResourceDesc");
			fprintf(stderr, "original_cudaGetTextureObjectResourceDesc:%p\n", original_cudaGetTextureObjectResourceDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetTextureObjectResourceDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetTextureObjectResourceDesc(
		pResDesc, 
		texObject
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}