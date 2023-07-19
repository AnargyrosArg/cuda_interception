#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemAllocManaged)(
CUdeviceptr *, 
size_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemAllocManaged(CUdeviceptr *dptr, size_t bytesize, unsigned int flags) {
		fprintf(stderr, "cuMemAllocManaged()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemAllocManaged)
		{
			//fetch the original function addr using dlsym
			original_cuMemAllocManaged = (CUresult (*)(
			CUdeviceptr *, 
			size_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuMemAllocManaged");
			fprintf(stderr, "original_cuMemAllocManaged:%p\n", original_cuMemAllocManaged);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemAllocManaged():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemAllocManaged(
		dptr, 
		bytesize, 
		flags
		);
	}
}