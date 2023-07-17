#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyDtoD_v2)(
CUdeviceptr, 
CUdeviceptr, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyDtoD_v2(CUdeviceptr dstDevice, CUdeviceptr srcDevice, size_t ByteCount) {
		fprintf(stderr, "cuMemcpyDtoD_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyDtoD_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyDtoD_v2 = (CUresult (*)(
			CUdeviceptr, 
			CUdeviceptr, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemcpyDtoD_v2");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyDtoD_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemcpyDtoD_v2(
		dstDevice, 
		srcDevice, 
		ByteCount
		);
	}
}