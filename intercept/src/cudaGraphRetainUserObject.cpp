#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphRetainUserObject)(
cudaGraph_t, 
cudaUserObject_t, 
unsigned int, 
unsigned int
);
extern "C"
{
	cudaError_t cudaGraphRetainUserObject(cudaGraph_t graph, cudaUserObject_t object, unsigned int count, unsigned int flags) {
		fprintf(stderr, "===============\ncudaGraphRetainUserObject()\n");
		char* __dlerror;
		if (!original_cudaGraphRetainUserObject)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphRetainUserObject = (cudaError_t (*)(
			cudaGraph_t, 
			cudaUserObject_t, 
			unsigned int, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaGraphRetainUserObject");
			fprintf(stderr, "original_cudaGraphRetainUserObject:%p\n", original_cudaGraphRetainUserObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphRetainUserObject():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphRetainUserObject(
		graph, 
		object, 
		count, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}