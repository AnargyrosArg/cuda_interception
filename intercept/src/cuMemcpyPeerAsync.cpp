#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyPeerAsync)(
CUdeviceptr, 
CUcontext, 
CUdeviceptr, 
CUcontext, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyPeerAsync(CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount, CUstream hStream) {
		fprintf(stderr, "cuMemcpyPeerAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyPeerAsync)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyPeerAsync = (CUresult (*)(
			CUdeviceptr, 
			CUcontext, 
			CUdeviceptr, 
			CUcontext, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemcpyPeerAsync");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyPeerAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemcpyPeerAsync(
		dstDevice, 
		dstContext, 
		srcDevice, 
		srcContext, 
		ByteCount, 
		hStream
		);
	}
}