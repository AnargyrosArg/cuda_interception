#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemUnmap)(
CUdeviceptr, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemUnmap(CUdeviceptr ptr, size_t size) {
		fprintf(stderr, "===============\ncuMemUnmap()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemUnmap)
		{
			//fetch the original function addr using dlsym
			original_cuMemUnmap = (CUresult (*)(
			CUdeviceptr, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemUnmap");
			fprintf(stderr, "original_cuMemUnmap:%p\n", original_cuMemUnmap);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemUnmap():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemUnmap(
		ptr, 
		size
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}