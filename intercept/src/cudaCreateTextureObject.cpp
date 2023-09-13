#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaCreateTextureObject)(
cudaTextureObject_t *, 
const struct cudaResourceDesc *, 
const struct cudaTextureDesc *, 
const struct cudaResourceViewDesc *
);
extern "C"
{
	cudaError_t cudaCreateTextureObject(cudaTextureObject_t *pTexObject, const struct cudaResourceDesc *pResDesc, const struct cudaTextureDesc *pTexDesc, const struct cudaResourceViewDesc *pResViewDesc) {
		fprintf(stderr, "===============\ncudaCreateTextureObject()\n");
		char* __dlerror;
		if (!original_cudaCreateTextureObject)
		{
			//fetch the original function addr using dlsym
			original_cudaCreateTextureObject = (cudaError_t (*)(
			cudaTextureObject_t *, 
			const struct cudaResourceDesc *, 
			const struct cudaTextureDesc *, 
			const struct cudaResourceViewDesc *)
			) dlsym(RTLD_NEXT, "cudaCreateTextureObject");
			fprintf(stderr, "original_cudaCreateTextureObject:%p\n", original_cudaCreateTextureObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaCreateTextureObject():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaCreateTextureObject(
		pTexObject, 
		pResDesc, 
		pTexDesc, 
		pResViewDesc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}