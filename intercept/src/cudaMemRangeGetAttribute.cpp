#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemRangeGetAttribute)(
void *, 
size_t, 
enum cudaMemRangeAttribute, 
const void *, 
size_t
);
extern "C"
{
	cudaError_t cudaMemRangeGetAttribute(void *data, size_t dataSize, enum cudaMemRangeAttribute attribute, const void *devPtr, size_t count) {
		fprintf(stderr, "===============\ncudaMemRangeGetAttribute()\n");
		char* __dlerror;
		if (!original_cudaMemRangeGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaMemRangeGetAttribute = (cudaError_t (*)(
			void *, 
			size_t, 
			enum cudaMemRangeAttribute, 
			const void *, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMemRangeGetAttribute");
			fprintf(stderr, "original_cudaMemRangeGetAttribute:%p\n", original_cudaMemRangeGetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemRangeGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemRangeGetAttribute(
		data, 
		dataSize, 
		attribute, 
		devPtr, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}