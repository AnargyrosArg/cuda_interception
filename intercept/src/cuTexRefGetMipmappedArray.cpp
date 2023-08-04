#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetMipmappedArray)(
CUmipmappedArray *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetMipmappedArray(CUmipmappedArray *phMipmappedArray, CUtexref hTexRef) {
		fprintf(stderr, "===============\ncuTexRefGetMipmappedArray()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetMipmappedArray)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetMipmappedArray = (CUresult (*)(
			CUmipmappedArray *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetMipmappedArray");
			fprintf(stderr, "original_cuTexRefGetMipmappedArray:%p\n", original_cuTexRefGetMipmappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetMipmappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuTexRefGetMipmappedArray(
		phMipmappedArray, 
		hTexRef
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}