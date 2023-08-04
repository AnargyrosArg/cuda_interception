#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyAtoD_v2)(
CUdeviceptr, 
CUarray, 
size_t, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyAtoD_v2(CUdeviceptr dstDevice, CUarray srcArray, size_t srcOffset, size_t ByteCount) {
		fprintf(stderr, "===============\ncuMemcpyAtoD_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyAtoD_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyAtoD_v2 = (CUresult (*)(
			CUdeviceptr, 
			CUarray, 
			size_t, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemcpyAtoD_v2");
			fprintf(stderr, "original_cuMemcpyAtoD_v2:%p\n", original_cuMemcpyAtoD_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyAtoD_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemcpyAtoD_v2(
		dstDevice, 
		srcArray, 
		srcOffset, 
		ByteCount
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}