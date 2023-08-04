#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemCreate)(
CUmemGenericAllocationHandle *, 
size_t, 
const CUmemAllocationProp *, 
unsigned long long
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemCreate(CUmemGenericAllocationHandle *handle, size_t size, const CUmemAllocationProp *prop, unsigned long long flags) {
		fprintf(stderr, "===============\ncuMemCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemCreate)
		{
			//fetch the original function addr using dlsym
			original_cuMemCreate = (CUresult (*)(
			CUmemGenericAllocationHandle *, 
			size_t, 
			const CUmemAllocationProp *, 
			unsigned long long)
			) dlsym(original_libcuda_handle, "cuMemCreate");
			fprintf(stderr, "original_cuMemCreate:%p\n", original_cuMemCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemCreate(
		handle, 
		size, 
		prop, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}