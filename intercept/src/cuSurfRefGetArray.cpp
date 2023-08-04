#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuSurfRefGetArray)(
CUarray *, 
CUsurfref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuSurfRefGetArray(CUarray *phArray, CUsurfref hSurfRef) {
		fprintf(stderr, "===============\ncuSurfRefGetArray()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuSurfRefGetArray)
		{
			//fetch the original function addr using dlsym
			original_cuSurfRefGetArray = (CUresult (*)(
			CUarray *, 
			CUsurfref)
			) dlsym(original_libcuda_handle, "cuSurfRefGetArray");
			fprintf(stderr, "original_cuSurfRefGetArray:%p\n", original_cuSurfRefGetArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuSurfRefGetArray():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuSurfRefGetArray(
		phArray, 
		hSurfRef
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}