#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemSetAccess)(
CUdeviceptr, 
size_t, 
const CUmemAccessDesc *, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemSetAccess(CUdeviceptr ptr, size_t size, const CUmemAccessDesc *desc, size_t count) {
		fprintf(stderr, "cuMemSetAccess()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemSetAccess)
		{
			//fetch the original function addr using dlsym
			original_cuMemSetAccess = (CUresult (*)(
			CUdeviceptr, 
			size_t, 
			const CUmemAccessDesc *, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemSetAccess");
			fprintf(stderr, "original_cuMemSetAccess:%p\n", original_cuMemSetAccess);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemSetAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemSetAccess(
		ptr, 
		size, 
		desc, 
		count
		);
	}
}