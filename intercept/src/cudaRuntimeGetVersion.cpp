#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaRuntimeGetVersion)(
int *
);
extern "C"
{
	cudaError_t cudaRuntimeGetVersion(int *runtimeVersion) {
		fprintf(stderr, "===============\ncudaRuntimeGetVersion()\n");
		char* __dlerror;
		if (!original_cudaRuntimeGetVersion)
		{
			//fetch the original function addr using dlsym
			original_cudaRuntimeGetVersion = (cudaError_t (*)(
			int *)
			) dlsym(RTLD_NEXT, "cudaRuntimeGetVersion");
			fprintf(stderr, "original_cudaRuntimeGetVersion:%p\n", original_cudaRuntimeGetVersion);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaRuntimeGetVersion():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaRuntimeGetVersion(
		runtimeVersion
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}