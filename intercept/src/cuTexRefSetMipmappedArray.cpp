#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetMipmappedArray)(
CUtexref, 
CUmipmappedArray, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetMipmappedArray(CUtexref hTexRef, CUmipmappedArray hMipmappedArray, unsigned int Flags) {
		fprintf(stderr, "cuTexRefSetMipmappedArray()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetMipmappedArray)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetMipmappedArray = (CUresult (*)(
			CUtexref, 
			CUmipmappedArray, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuTexRefSetMipmappedArray");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetMipmappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefSetMipmappedArray(
		hTexRef, 
		hMipmappedArray, 
		Flags
		);
	}
}