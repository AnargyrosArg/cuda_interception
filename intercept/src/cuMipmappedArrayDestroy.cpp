#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMipmappedArrayDestroy)(
CUmipmappedArray
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMipmappedArrayDestroy(CUmipmappedArray hMipmappedArray) {
		fprintf(stderr, "cuMipmappedArrayDestroy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMipmappedArrayDestroy)
		{
			//fetch the original function addr using dlsym
			original_cuMipmappedArrayDestroy = (CUresult (*)(
			CUmipmappedArray)
			) dlsym(original_libcuda_handle, "cuMipmappedArrayDestroy");
			fprintf(stderr, "original_cuMipmappedArrayDestroy:%p\n", original_cuMipmappedArrayDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMipmappedArrayDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMipmappedArrayDestroy(
		hMipmappedArray
		);
	}
}