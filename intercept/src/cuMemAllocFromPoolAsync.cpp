#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemAllocFromPoolAsync)(
CUdeviceptr *, 
size_t, 
CUmemoryPool, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemAllocFromPoolAsync(CUdeviceptr *dptr, size_t bytesize, CUmemoryPool pool, CUstream hStream) {
		fprintf(stderr, "===============\ncuMemAllocFromPoolAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemAllocFromPoolAsync)
		{
			//fetch the original function addr using dlsym
			original_cuMemAllocFromPoolAsync = (CUresult (*)(
			CUdeviceptr *, 
			size_t, 
			CUmemoryPool, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemAllocFromPoolAsync");
			fprintf(stderr, "original_cuMemAllocFromPoolAsync:%p\n", original_cuMemAllocFromPoolAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemAllocFromPoolAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemAllocFromPoolAsync(
		dptr, 
		bytesize, 
		pool, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}