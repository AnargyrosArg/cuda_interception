#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetPCIBusId)(
char *, 
int, 
int
);
extern "C"
{
	cudaError_t cudaDeviceGetPCIBusId(char *pciBusId, int len, int device) {
		fprintf(stderr, "===============\ncudaDeviceGetPCIBusId()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetPCIBusId)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetPCIBusId = (cudaError_t (*)(
			char *, 
			int, 
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceGetPCIBusId");
			fprintf(stderr, "original_cudaDeviceGetPCIBusId:%p\n", original_cudaDeviceGetPCIBusId);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetPCIBusId():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetPCIBusId(
		pciBusId, 
		len, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}