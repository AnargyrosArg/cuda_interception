#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetAddressMode)(
CUaddress_mode *, 
CUtexref, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetAddressMode(CUaddress_mode *pam, CUtexref hTexRef, int dim) {
		fprintf(stderr, "cuTexRefGetAddressMode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetAddressMode)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetAddressMode = (CUresult (*)(
			CUaddress_mode *, 
			CUtexref, 
			int)
			) dlsym(original_libcuda_handle, "cuTexRefGetAddressMode");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetAddressMode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefGetAddressMode(
		pam, 
		hTexRef, 
		dim
		);
	}
}