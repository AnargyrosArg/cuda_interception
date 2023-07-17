#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuSurfRefSetArray)(
CUsurfref, 
CUarray, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuSurfRefSetArray(CUsurfref hSurfRef, CUarray hArray, unsigned int Flags) {
		fprintf(stderr, "cuSurfRefSetArray()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuSurfRefSetArray)
		{
			//fetch the original function addr using dlsym
			original_cuSurfRefSetArray = (CUresult (*)(
			CUsurfref, 
			CUarray, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuSurfRefSetArray");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuSurfRefSetArray():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuSurfRefSetArray(
		hSurfRef, 
		hArray, 
		Flags
		);
	}
}