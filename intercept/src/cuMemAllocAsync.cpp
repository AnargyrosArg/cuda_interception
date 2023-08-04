#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemAllocAsync)(
CUdeviceptr *, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemAllocAsync(CUdeviceptr *dptr, size_t bytesize, CUstream hStream) {
		fprintf(stderr, "===============\ncuMemAllocAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemAllocAsync)
		{
			//fetch the original function addr using dlsym
			original_cuMemAllocAsync = (CUresult (*)(
			CUdeviceptr *, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemAllocAsync");
			fprintf(stderr, "original_cuMemAllocAsync:%p\n", original_cuMemAllocAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemAllocAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemAllocAsync(
		dptr, 
		bytesize, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}