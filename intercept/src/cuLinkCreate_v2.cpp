#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLinkCreate_v2)(
unsigned int, 
CUjit_option *, 
void **, 
CUlinkState *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLinkCreate_v2(unsigned int numOptions, CUjit_option *options, void **optionValues, CUlinkState *stateOut) {
		fprintf(stderr, "cuLinkCreate_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLinkCreate_v2)
		{
			//fetch the original function addr using dlsym
			original_cuLinkCreate_v2 = (CUresult (*)(
			unsigned int, 
			CUjit_option *, 
			void **, 
			CUlinkState *)
			) dlsym(original_libcuda_handle, "cuLinkCreate_v2");
			fprintf(stderr, "original_cuLinkCreate_v2:%p\n", original_cuLinkCreate_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLinkCreate_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLinkCreate_v2(
		numOptions, 
		options, 
		optionValues, 
		stateOut
		);
	}
}