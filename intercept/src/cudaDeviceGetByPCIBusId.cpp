#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetByPCIBusId)(
int *, 
const char *
);
extern "C"
{
	cudaError_t cudaDeviceGetByPCIBusId(int *device, const char *pciBusId) {
		fprintf(stderr, "===============\ncudaDeviceGetByPCIBusId()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetByPCIBusId)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetByPCIBusId = (cudaError_t (*)(
			int *, 
			const char *)
			) dlsym(RTLD_NEXT, "cudaDeviceGetByPCIBusId");
			fprintf(stderr, "original_cudaDeviceGetByPCIBusId:%p\n", original_cudaDeviceGetByPCIBusId);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetByPCIBusId():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetByPCIBusId(
		device, 
		pciBusId
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}