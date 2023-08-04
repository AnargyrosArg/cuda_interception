#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyDtoHAsync_v2)(
void *, 
CUdeviceptr, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyDtoHAsync_v2(void *dstHost, CUdeviceptr srcDevice, size_t ByteCount, CUstream hStream) {
		fprintf(stderr, "===============\ncuMemcpyDtoHAsync_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyDtoHAsync_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyDtoHAsync_v2 = (CUresult (*)(
			void *, 
			CUdeviceptr, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemcpyDtoHAsync_v2");
			fprintf(stderr, "original_cuMemcpyDtoHAsync_v2:%p\n", original_cuMemcpyDtoHAsync_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyDtoHAsync_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemcpyDtoHAsync_v2(
		dstHost, 
		srcDevice, 
		ByteCount, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}