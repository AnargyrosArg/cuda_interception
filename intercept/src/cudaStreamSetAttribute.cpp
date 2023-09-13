#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamSetAttribute)(
cudaStream_t, 
cudaStreamAttrID, 
const cudaStreamAttrValue *
);
extern "C"
{
	cudaError_t cudaStreamSetAttribute(cudaStream_t hStream, cudaStreamAttrID attr, const cudaStreamAttrValue *value) {
		fprintf(stderr, "===============\ncudaStreamSetAttribute()\n");
		char* __dlerror;
		if (!original_cudaStreamSetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamSetAttribute = (cudaError_t (*)(
			cudaStream_t, 
			cudaStreamAttrID, 
			const cudaStreamAttrValue *)
			) dlsym(RTLD_NEXT, "cudaStreamSetAttribute");
			fprintf(stderr, "original_cudaStreamSetAttribute:%p\n", original_cudaStreamSetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamSetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamSetAttribute(
		hStream, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}