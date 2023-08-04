#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemExportToShareableHandle)(
void *, 
CUmemGenericAllocationHandle, 
CUmemAllocationHandleType, 
unsigned long long
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemExportToShareableHandle(void *shareableHandle, CUmemGenericAllocationHandle handle, CUmemAllocationHandleType handleType, unsigned long long flags) {
		fprintf(stderr, "===============\ncuMemExportToShareableHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemExportToShareableHandle)
		{
			//fetch the original function addr using dlsym
			original_cuMemExportToShareableHandle = (CUresult (*)(
			void *, 
			CUmemGenericAllocationHandle, 
			CUmemAllocationHandleType, 
			unsigned long long)
			) dlsym(original_libcuda_handle, "cuMemExportToShareableHandle");
			fprintf(stderr, "original_cuMemExportToShareableHandle:%p\n", original_cuMemExportToShareableHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemExportToShareableHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemExportToShareableHandle(
		shareableHandle, 
		handle, 
		handleType, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}