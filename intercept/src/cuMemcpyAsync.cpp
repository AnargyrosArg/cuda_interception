#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyAsync)(
CUdeviceptr, 
CUdeviceptr, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyAsync(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount, CUstream hStream) {
		fprintf(stderr, "cuMemcpyAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyAsync)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyAsync = (CUresult (*)(
			CUdeviceptr, 
			CUdeviceptr, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemcpyAsync");
			fprintf(stderr, "original_cuMemcpyAsync:%p\n", original_cuMemcpyAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemcpyAsync(
		dst, 
		src, 
		ByteCount, 
		hStream
		);
	}
}