#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaUserObjectRelease)(
cudaUserObject_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaUserObjectRelease(cudaUserObject_t object, unsigned int count) {
		fprintf(stderr, "===============\ncudaUserObjectRelease()\n");
		char* __dlerror;
		if (!original_cudaUserObjectRelease)
		{
			//fetch the original function addr using dlsym
			original_cudaUserObjectRelease = (cudaError_t (*)(
			cudaUserObject_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaUserObjectRelease");
			fprintf(stderr, "original_cudaUserObjectRelease:%p\n", original_cudaUserObjectRelease);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaUserObjectRelease():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaUserObjectRelease(
		object, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}