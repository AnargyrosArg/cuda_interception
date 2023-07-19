#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemImportFromShareableHandle)(
CUmemGenericAllocationHandle *, 
void *, 
CUmemAllocationHandleType
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemImportFromShareableHandle(CUmemGenericAllocationHandle *handle, void *osHandle, CUmemAllocationHandleType shHandleType) {
		fprintf(stderr, "cuMemImportFromShareableHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemImportFromShareableHandle)
		{
			//fetch the original function addr using dlsym
			original_cuMemImportFromShareableHandle = (CUresult (*)(
			CUmemGenericAllocationHandle *, 
			void *, 
			CUmemAllocationHandleType)
			) dlsym(original_libcuda_handle, "cuMemImportFromShareableHandle");
			fprintf(stderr, "original_cuMemImportFromShareableHandle:%p\n", original_cuMemImportFromShareableHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemImportFromShareableHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemImportFromShareableHandle(
		handle, 
		osHandle, 
		shHandleType
		);
	}
}