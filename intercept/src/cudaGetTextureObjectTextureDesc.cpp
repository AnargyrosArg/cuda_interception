#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetTextureObjectTextureDesc)(
struct cudaTextureDesc *, 
cudaTextureObject_t
);
extern "C"
{
	cudaError_t cudaGetTextureObjectTextureDesc(struct cudaTextureDesc *pTexDesc, cudaTextureObject_t texObject) {
		fprintf(stderr, "===============\ncudaGetTextureObjectTextureDesc()\n");
		char* __dlerror;
		if (!original_cudaGetTextureObjectTextureDesc)
		{
			//fetch the original function addr using dlsym
			original_cudaGetTextureObjectTextureDesc = (cudaError_t (*)(
			struct cudaTextureDesc *, 
			cudaTextureObject_t)
			) dlsym(RTLD_NEXT, "cudaGetTextureObjectTextureDesc");
			fprintf(stderr, "original_cudaGetTextureObjectTextureDesc:%p\n", original_cudaGetTextureObjectTextureDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetTextureObjectTextureDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetTextureObjectTextureDesc(
		pTexDesc, 
		texObject
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}