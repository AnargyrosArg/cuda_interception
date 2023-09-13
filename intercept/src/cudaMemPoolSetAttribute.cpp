#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolSetAttribute)(
cudaMemPool_t, 
enum cudaMemPoolAttr, 
void *
);
extern "C"
{
	cudaError_t cudaMemPoolSetAttribute(cudaMemPool_t memPool, enum cudaMemPoolAttr attr, void *value) {
		fprintf(stderr, "===============\ncudaMemPoolSetAttribute()\n");
		char* __dlerror;
		if (!original_cudaMemPoolSetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolSetAttribute = (cudaError_t (*)(
			cudaMemPool_t, 
			enum cudaMemPoolAttr, 
			void *)
			) dlsym(RTLD_NEXT, "cudaMemPoolSetAttribute");
			fprintf(stderr, "original_cudaMemPoolSetAttribute:%p\n", original_cudaMemPoolSetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolSetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolSetAttribute(
		memPool, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}