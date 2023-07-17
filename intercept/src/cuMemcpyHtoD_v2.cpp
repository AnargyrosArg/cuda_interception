#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyHtoD_v2)(
CUdeviceptr, 
const void *, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyHtoD_v2(CUdeviceptr dstDevice, const void *srcHost, size_t ByteCount) {
		fprintf(stderr, "cuMemcpyHtoD_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyHtoD_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyHtoD_v2 = (CUresult (*)(
			CUdeviceptr, 
			const void *, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemcpyHtoD_v2");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyHtoD_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemcpyHtoD_v2(
		dstDevice, 
		srcHost, 
		ByteCount
		);
	}
}