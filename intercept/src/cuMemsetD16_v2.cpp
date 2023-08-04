#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemsetD16_v2)(
CUdeviceptr, 
unsigned short, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemsetD16_v2(CUdeviceptr dstDevice, unsigned short us, size_t N) {
		fprintf(stderr, "===============\ncuMemsetD16_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemsetD16_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemsetD16_v2 = (CUresult (*)(
			CUdeviceptr, 
			unsigned short, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemsetD16_v2");
			fprintf(stderr, "original_cuMemsetD16_v2:%p\n", original_cuMemsetD16_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemsetD16_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemsetD16_v2(
		dstDevice, 
		us, 
		N
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}