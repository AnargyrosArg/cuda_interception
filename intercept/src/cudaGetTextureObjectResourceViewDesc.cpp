#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetTextureObjectResourceViewDesc)(
struct cudaResourceViewDesc *, 
cudaTextureObject_t
);
extern "C"
{
	cudaError_t cudaGetTextureObjectResourceViewDesc(struct cudaResourceViewDesc *pResViewDesc, cudaTextureObject_t texObject) {
		fprintf(stderr, "===============\ncudaGetTextureObjectResourceViewDesc()\n");
		char* __dlerror;
		if (!original_cudaGetTextureObjectResourceViewDesc)
		{
			//fetch the original function addr using dlsym
			original_cudaGetTextureObjectResourceViewDesc = (cudaError_t (*)(
			struct cudaResourceViewDesc *, 
			cudaTextureObject_t)
			) dlsym(RTLD_NEXT, "cudaGetTextureObjectResourceViewDesc");
			fprintf(stderr, "original_cudaGetTextureObjectResourceViewDesc:%p\n", original_cudaGetTextureObjectResourceViewDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetTextureObjectResourceViewDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetTextureObjectResourceViewDesc(
		pResViewDesc, 
		texObject
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}