#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuModuleGetTexRef)(
CUtexref *, 
CUmodule, 
const char *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuModuleGetTexRef(CUtexref *pTexRef, CUmodule hmod, const char *name) {
		fprintf(stderr, "cuModuleGetTexRef()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuModuleGetTexRef)
		{
			//fetch the original function addr using dlsym
			original_cuModuleGetTexRef = (CUresult (*)(
			CUtexref *, 
			CUmodule, 
			const char *)
			) dlsym(original_libcuda_handle, "cuModuleGetTexRef");
			fprintf(stderr, "original_cuModuleGetTexRef:%p\n", original_cuModuleGetTexRef);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuModuleGetTexRef():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuModuleGetTexRef(
		pTexRef, 
		hmod, 
		name
		);
	}
}