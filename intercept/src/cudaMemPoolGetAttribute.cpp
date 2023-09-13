#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolGetAttribute)(
cudaMemPool_t, 
enum cudaMemPoolAttr, 
void *
);
extern "C"
{
	cudaError_t cudaMemPoolGetAttribute(cudaMemPool_t memPool, enum cudaMemPoolAttr attr, void *value) {
		fprintf(stderr, "===============\ncudaMemPoolGetAttribute()\n");
		char* __dlerror;
		if (!original_cudaMemPoolGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolGetAttribute = (cudaError_t (*)(
			cudaMemPool_t, 
			enum cudaMemPoolAttr, 
			void *)
			) dlsym(RTLD_NEXT, "cudaMemPoolGetAttribute");
			fprintf(stderr, "original_cudaMemPoolGetAttribute:%p\n", original_cudaMemPoolGetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolGetAttribute(
		memPool, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}