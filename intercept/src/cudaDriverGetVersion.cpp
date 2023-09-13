#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDriverGetVersion)(
int *
);
extern "C"
{
	cudaError_t cudaDriverGetVersion(int *driverVersion) {
		fprintf(stderr, "===============\ncudaDriverGetVersion()\n");
		char* __dlerror;
		if (!original_cudaDriverGetVersion)
		{
			//fetch the original function addr using dlsym
			original_cudaDriverGetVersion = (cudaError_t (*)(
			int *)
			) dlsym(RTLD_NEXT, "cudaDriverGetVersion");
			fprintf(stderr, "original_cudaDriverGetVersion:%p\n", original_cudaDriverGetVersion);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDriverGetVersion():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDriverGetVersion(
		driverVersion
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}