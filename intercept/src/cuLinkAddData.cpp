#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLinkAddData)(
CUlinkState, 
CUjitInputType, 
void *, 
size_t, 
const char *, 
unsigned int, 
CUjit_option *, 
void **
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;

//cuLinkAddData is defined as cuLinkAddData_v2 in cuda.h
#undef cuLinkAddData


extern "C"
{
	CUresult cuLinkAddData(CUlinkState state, CUjitInputType type, void *data, size_t size, const char *name, unsigned int numOptions, CUjit_option *options, void **optionValues) {
		fprintf(stderr, "===============\ncuLinkAddData()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLinkAddData)
		{
			//fetch the original function addr using dlsym
			original_cuLinkAddData = (CUresult (*)(
			CUlinkState, 
			CUjitInputType, 
			void *, 
			size_t, 
			const char *, 
			unsigned int, 
			CUjit_option *, 
			void **)
			) dlsym(original_libcuda_handle, "cuLinkAddData");
			fprintf(stderr, "original_cuLinkAddData:%p\n", original_cuLinkAddData);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLinkAddData():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuLinkAddData(
		state, 
		type, 
		data, 
		size, 
		name, 
		numOptions, 
		options, 
		optionValues
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}