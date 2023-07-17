#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefCreate)(
CUtexref *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefCreate(CUtexref *pTexRef) {
		fprintf(stderr, "cuTexRefCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefCreate)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefCreate = (CUresult (*)(
			CUtexref *)
			) dlsym(original_libcuda_handle, "cuTexRefCreate");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefCreate(
		pTexRef
		);
	}
}