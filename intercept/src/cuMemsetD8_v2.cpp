#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemsetD8_v2)(
CUdeviceptr, 
unsigned char, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemsetD8_v2(CUdeviceptr dstDevice, unsigned char uc, size_t N) {
		fprintf(stderr, "cuMemsetD8_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemsetD8_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemsetD8_v2 = (CUresult (*)(
			CUdeviceptr, 
			unsigned char, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemsetD8_v2");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemsetD8_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemsetD8_v2(
		dstDevice, 
		uc, 
		N
		);
	}
}