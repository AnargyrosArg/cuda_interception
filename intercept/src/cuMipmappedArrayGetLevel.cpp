#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMipmappedArrayGetLevel)(
CUarray *, 
CUmipmappedArray, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMipmappedArrayGetLevel(CUarray *pLevelArray, CUmipmappedArray hMipmappedArray, unsigned int level) {
		fprintf(stderr, "===============\ncuMipmappedArrayGetLevel()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMipmappedArrayGetLevel)
		{
			//fetch the original function addr using dlsym
			original_cuMipmappedArrayGetLevel = (CUresult (*)(
			CUarray *, 
			CUmipmappedArray, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuMipmappedArrayGetLevel");
			fprintf(stderr, "original_cuMipmappedArrayGetLevel:%p\n", original_cuMipmappedArrayGetLevel);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMipmappedArrayGetLevel():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMipmappedArrayGetLevel(
		pLevelArray, 
		hMipmappedArray, 
		level
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}