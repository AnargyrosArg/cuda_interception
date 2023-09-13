#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphReleaseUserObject)(
cudaGraph_t, 
cudaUserObject_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaGraphReleaseUserObject(cudaGraph_t graph, cudaUserObject_t object, unsigned int count) {
		fprintf(stderr, "===============\ncudaGraphReleaseUserObject()\n");
		char* __dlerror;
		if (!original_cudaGraphReleaseUserObject)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphReleaseUserObject = (cudaError_t (*)(
			cudaGraph_t, 
			cudaUserObject_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaGraphReleaseUserObject");
			fprintf(stderr, "original_cudaGraphReleaseUserObject:%p\n", original_cudaGraphReleaseUserObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphReleaseUserObject():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphReleaseUserObject(
		graph, 
		object, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}