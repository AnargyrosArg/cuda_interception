#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemGetAddressRange_v2)(
CUdeviceptr *, 
size_t *, 
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemGetAddressRange_v2(CUdeviceptr *pbase, size_t *psize, CUdeviceptr dptr) {
		fprintf(stderr, "===============\ncuMemGetAddressRange_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemGetAddressRange_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemGetAddressRange_v2 = (CUresult (*)(
			CUdeviceptr *, 
			size_t *, 
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuMemGetAddressRange_v2");
			fprintf(stderr, "original_cuMemGetAddressRange_v2:%p\n", original_cuMemGetAddressRange_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemGetAddressRange_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemGetAddressRange_v2(
		pbase, 
		psize, 
		dptr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}