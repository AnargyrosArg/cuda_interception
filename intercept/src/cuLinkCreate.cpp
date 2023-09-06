#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLinkCreate)(
unsigned int, 
CUjit_option *, 
void **, 
CUlinkState *
);


//cuLinkCreate is defined as cuLinkCreate in cuda.h
#undef cuLinkCreate


//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLinkCreate(unsigned int numOptions, CUjit_option *options, void **optionValues, CUlinkState *stateOut) {
		fprintf(stderr, "===============\ncuLinkCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLinkCreate)
		{
			//fetch the original function addr using dlsym
			original_cuLinkCreate = (CUresult (*)(
			unsigned int, 
			CUjit_option *, 
			void **, 
			CUlinkState *)
			) dlsym(original_libcuda_handle, "cuLinkCreate");
			fprintf(stderr, "original_cuLinkCreate:%p\n", original_cuLinkCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLinkCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuLinkCreate(
		numOptions, 
		options, 
		optionValues, 
		stateOut
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}