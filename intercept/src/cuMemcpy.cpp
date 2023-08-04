#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpy)(
CUdeviceptr, 
CUdeviceptr, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpy(CUdeviceptr dst, CUdeviceptr src, size_t ByteCount) {
		fprintf(stderr, "===============\ncuMemcpy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpy)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpy = (CUresult (*)(
			CUdeviceptr, 
			CUdeviceptr, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemcpy");
			fprintf(stderr, "original_cuMemcpy:%p\n", original_cuMemcpy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpy():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemcpy(
		dst, 
		src, 
		ByteCount
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}