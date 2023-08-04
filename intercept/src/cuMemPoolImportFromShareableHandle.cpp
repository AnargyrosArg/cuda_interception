#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolImportFromShareableHandle)(
CUmemoryPool *, 
void *, 
CUmemAllocationHandleType, 
unsigned long long
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolImportFromShareableHandle(CUmemoryPool *pool_out, void *handle, CUmemAllocationHandleType handleType, unsigned long long flags) {
		fprintf(stderr, "===============\ncuMemPoolImportFromShareableHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolImportFromShareableHandle)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolImportFromShareableHandle = (CUresult (*)(
			CUmemoryPool *, 
			void *, 
			CUmemAllocationHandleType, 
			unsigned long long)
			) dlsym(original_libcuda_handle, "cuMemPoolImportFromShareableHandle");
			fprintf(stderr, "original_cuMemPoolImportFromShareableHandle:%p\n", original_cuMemPoolImportFromShareableHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolImportFromShareableHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemPoolImportFromShareableHandle(
		pool_out, 
		handle, 
		handleType, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}