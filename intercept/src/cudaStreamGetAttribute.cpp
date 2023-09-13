#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamGetAttribute)(
cudaStream_t, 
cudaStreamAttrID, 
cudaStreamAttrValue *
);
extern "C"
{
	cudaError_t cudaStreamGetAttribute(cudaStream_t hStream, cudaStreamAttrID attr, cudaStreamAttrValue *value_out) {
		fprintf(stderr, "===============\ncudaStreamGetAttribute()\n");
		char* __dlerror;
		if (!original_cudaStreamGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamGetAttribute = (cudaError_t (*)(
			cudaStream_t, 
			cudaStreamAttrID, 
			cudaStreamAttrValue *)
			) dlsym(RTLD_NEXT, "cudaStreamGetAttribute");
			fprintf(stderr, "original_cudaStreamGetAttribute:%p\n", original_cudaStreamGetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamGetAttribute(
		hStream, 
		attr, 
		value_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}