#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyPeer)(
CUdeviceptr, 
CUcontext, 
CUdeviceptr, 
CUcontext, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyPeer(CUdeviceptr dstDevice, CUcontext dstContext, CUdeviceptr srcDevice, CUcontext srcContext, size_t ByteCount) {
		fprintf(stderr, "cuMemcpyPeer()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyPeer)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyPeer = (CUresult (*)(
			CUdeviceptr, 
			CUcontext, 
			CUdeviceptr, 
			CUcontext, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemcpyPeer");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyPeer():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemcpyPeer(
		dstDevice, 
		dstContext, 
		srcDevice, 
		srcContext, 
		ByteCount
		);
	}
}