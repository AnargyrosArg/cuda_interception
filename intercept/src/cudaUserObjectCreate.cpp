#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaUserObjectCreate)(
cudaUserObject_t *, 
void *, 
cudaHostFn_t, 
unsigned int, 
unsigned int
);
extern "C"
{
	cudaError_t cudaUserObjectCreate(cudaUserObject_t *object_out, void *ptr, cudaHostFn_t destroy, unsigned int initialRefcount, unsigned int flags) {
		fprintf(stderr, "===============\ncudaUserObjectCreate()\n");
		char* __dlerror;
		if (!original_cudaUserObjectCreate)
		{
			//fetch the original function addr using dlsym
			original_cudaUserObjectCreate = (cudaError_t (*)(
			cudaUserObject_t *, 
			void *, 
			cudaHostFn_t, 
			unsigned int, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaUserObjectCreate");
			fprintf(stderr, "original_cudaUserObjectCreate:%p\n", original_cudaUserObjectCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaUserObjectCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaUserObjectCreate(
		object_out, 
		ptr, 
		destroy, 
		initialRefcount, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}