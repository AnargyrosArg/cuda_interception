#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolExportToShareableHandle)(
void *, 
CUmemoryPool, 
CUmemAllocationHandleType, 
unsigned long long
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolExportToShareableHandle(void *handle_out, CUmemoryPool pool, CUmemAllocationHandleType handleType, unsigned long long flags) {
		fprintf(stderr, "cuMemPoolExportToShareableHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolExportToShareableHandle)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolExportToShareableHandle = (CUresult (*)(
			void *, 
			CUmemoryPool, 
			CUmemAllocationHandleType, 
			unsigned long long)
			) dlsym(original_libcuda_handle, "cuMemPoolExportToShareableHandle");
			fprintf(stderr, "original_cuMemPoolExportToShareableHandle:%p\n", original_cuMemPoolExportToShareableHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolExportToShareableHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemPoolExportToShareableHandle(
		handle_out, 
		pool, 
		handleType, 
		flags
		);
	}
}