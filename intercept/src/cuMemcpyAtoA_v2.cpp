#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyAtoA_v2)(
CUarray, 
size_t, 
CUarray, 
size_t, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyAtoA_v2(CUarray dstArray, size_t dstOffset, CUarray srcArray, size_t srcOffset, size_t ByteCount) {
		fprintf(stderr, "cuMemcpyAtoA_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyAtoA_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyAtoA_v2 = (CUresult (*)(
			CUarray, 
			size_t, 
			CUarray, 
			size_t, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemcpyAtoA_v2");
			fprintf(stderr, "original_cuMemcpyAtoA_v2:%p\n", original_cuMemcpyAtoA_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyAtoA_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemcpyAtoA_v2(
		dstArray, 
		dstOffset, 
		srcArray, 
		srcOffset, 
		ByteCount
		);
	}
}