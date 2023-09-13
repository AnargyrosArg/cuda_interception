#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDestroyTextureObject)(
cudaTextureObject_t
);
extern "C"
{
	cudaError_t cudaDestroyTextureObject(cudaTextureObject_t texObject) {
		fprintf(stderr, "===============\ncudaDestroyTextureObject()\n");
		char* __dlerror;
		if (!original_cudaDestroyTextureObject)
		{
			//fetch the original function addr using dlsym
			original_cudaDestroyTextureObject = (cudaError_t (*)(
			cudaTextureObject_t)
			) dlsym(RTLD_NEXT, "cudaDestroyTextureObject");
			fprintf(stderr, "original_cudaDestroyTextureObject:%p\n", original_cudaDestroyTextureObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDestroyTextureObject():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDestroyTextureObject(
		texObject
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}