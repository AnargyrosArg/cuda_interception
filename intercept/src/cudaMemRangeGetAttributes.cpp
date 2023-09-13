#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemRangeGetAttributes)(
void **, 
size_t *, 
enum cudaMemRangeAttribute *, 
size_t, 
const void *, 
size_t
);
extern "C"
{
	cudaError_t cudaMemRangeGetAttributes(void **data, size_t *dataSizes, enum cudaMemRangeAttribute *attributes, size_t numAttributes, const void *devPtr, size_t count) {
		fprintf(stderr, "===============\ncudaMemRangeGetAttributes()\n");
		char* __dlerror;
		if (!original_cudaMemRangeGetAttributes)
		{
			//fetch the original function addr using dlsym
			original_cudaMemRangeGetAttributes = (cudaError_t (*)(
			void **, 
			size_t *, 
			enum cudaMemRangeAttribute *, 
			size_t, 
			const void *, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMemRangeGetAttributes");
			fprintf(stderr, "original_cudaMemRangeGetAttributes:%p\n", original_cudaMemRangeGetAttributes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemRangeGetAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemRangeGetAttributes(
		data, 
		dataSizes, 
		attributes, 
		numAttributes, 
		devPtr, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}