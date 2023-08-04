#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemRetainAllocationHandle)(
CUmemGenericAllocationHandle *, 
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemRetainAllocationHandle(CUmemGenericAllocationHandle *handle, void *addr) {
		fprintf(stderr, "===============\ncuMemRetainAllocationHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemRetainAllocationHandle)
		{
			//fetch the original function addr using dlsym
			original_cuMemRetainAllocationHandle = (CUresult (*)(
			CUmemGenericAllocationHandle *, 
			void *)
			) dlsym(original_libcuda_handle, "cuMemRetainAllocationHandle");
			fprintf(stderr, "original_cuMemRetainAllocationHandle:%p\n", original_cuMemRetainAllocationHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemRetainAllocationHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemRetainAllocationHandle(
		handle, 
		addr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}