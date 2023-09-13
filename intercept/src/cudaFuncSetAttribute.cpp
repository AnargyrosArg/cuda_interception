#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaFuncSetAttribute)(
const void *, 
enum cudaFuncAttribute, 
int
);
extern "C"
{
	cudaError_t cudaFuncSetAttribute(const void *func, enum cudaFuncAttribute attr, int value) {
		fprintf(stderr, "===============\ncudaFuncSetAttribute()\n");
		char* __dlerror;
		if (!original_cudaFuncSetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaFuncSetAttribute = (cudaError_t (*)(
			const void *, 
			enum cudaFuncAttribute, 
			int)
			) dlsym(RTLD_NEXT, "cudaFuncSetAttribute");
			fprintf(stderr, "original_cudaFuncSetAttribute:%p\n", original_cudaFuncSetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaFuncSetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaFuncSetAttribute(
		func, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}