#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLinkAddFile)(
CUlinkState, 
CUjitInputType, 
const char *, 
unsigned int, 
CUjit_option *, 
void **
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;


//cuLinkAddFile is defined as cuLinkAddFile_v2 in cuda.h
#undef cuLinkAddFile


extern "C"
{
	CUresult cuLinkAddFile(CUlinkState state, CUjitInputType type, const char *path, unsigned int numOptions, CUjit_option *options, void **optionValues) {
		fprintf(stderr, "===============\ncuLinkAddFile()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLinkAddFile)
		{
			//fetch the original function addr using dlsym
			original_cuLinkAddFile = (CUresult (*)(
			CUlinkState, 
			CUjitInputType, 
			const char *, 
			unsigned int, 
			CUjit_option *, 
			void **)
			) dlsym(original_libcuda_handle, "cuLinkAddFile");
			fprintf(stderr, "original_cuLinkAddFile:%p\n", original_cuLinkAddFile);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLinkAddFile():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuLinkAddFile(
		state, 
		type, 
		path, 
		numOptions, 
		options, 
		optionValues
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}