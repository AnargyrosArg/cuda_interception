#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLinkAddFile_v2)(
CUlinkState, 
CUjitInputType, 
const char *, 
unsigned int, 
CUjit_option *, 
void **
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLinkAddFile_v2(CUlinkState state, CUjitInputType type, const char *path, unsigned int numOptions, CUjit_option *options, void **optionValues) {
		fprintf(stderr, "cuLinkAddFile_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLinkAddFile_v2)
		{
			//fetch the original function addr using dlsym
			original_cuLinkAddFile_v2 = (CUresult (*)(
			CUlinkState, 
			CUjitInputType, 
			const char *, 
			unsigned int, 
			CUjit_option *, 
			void **)
			) dlsym(original_libcuda_handle, "cuLinkAddFile_v2");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLinkAddFile_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLinkAddFile_v2(
		state, 
		type, 
		path, 
		numOptions, 
		options, 
		optionValues
		);
	}
}