#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuArrayGetPlane)(
CUarray *, 
CUarray, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuArrayGetPlane(CUarray *pPlaneArray, CUarray hArray, unsigned int planeIdx) {
		fprintf(stderr, "cuArrayGetPlane()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuArrayGetPlane)
		{
			//fetch the original function addr using dlsym
			original_cuArrayGetPlane = (CUresult (*)(
			CUarray *, 
			CUarray, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuArrayGetPlane");
			fprintf(stderr, "original_cuArrayGetPlane:%p\n", original_cuArrayGetPlane);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuArrayGetPlane():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuArrayGetPlane(
		pPlaneArray, 
		hArray, 
		planeIdx
		);
	}
}