#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemAdvise)(
CUdeviceptr, 
size_t, 
CUmem_advise, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemAdvise(CUdeviceptr devPtr, size_t count, CUmem_advise advice, CUdevice device) {
		fprintf(stderr, "===============\ncuMemAdvise()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemAdvise)
		{
			//fetch the original function addr using dlsym
			original_cuMemAdvise = (CUresult (*)(
			CUdeviceptr, 
			size_t, 
			CUmem_advise, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuMemAdvise");
			fprintf(stderr, "original_cuMemAdvise:%p\n", original_cuMemAdvise);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemAdvise():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemAdvise(
		devPtr, 
		count, 
		advice, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}