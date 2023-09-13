#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaUserObjectRetain)(
cudaUserObject_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaUserObjectRetain(cudaUserObject_t object, unsigned int count) {
		fprintf(stderr, "===============\ncudaUserObjectRetain()\n");
		char* __dlerror;
		if (!original_cudaUserObjectRetain)
		{
			//fetch the original function addr using dlsym
			original_cudaUserObjectRetain = (cudaError_t (*)(
			cudaUserObject_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaUserObjectRetain");
			fprintf(stderr, "original_cudaUserObjectRetain:%p\n", original_cudaUserObjectRetain);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaUserObjectRetain():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaUserObjectRetain(
		object, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}