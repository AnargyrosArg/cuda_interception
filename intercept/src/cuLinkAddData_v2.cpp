#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLinkAddData_v2)(
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
extern "C"
{
	CUresult cuLinkAddData_v2(CUlinkState state, CUjitInputType type, void *data, size_t size, const char *name, unsigned int numOptions, CUjit_option *options, void **optionValues) {
		fprintf(stderr, "cuLinkAddData_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLinkAddData_v2)
		{
			//fetch the original function addr using dlsym
			original_cuLinkAddData_v2 = (CUresult (*)(
			CUlinkState, 
			CUjitInputType, 
			void *, 
			size_t, 
			const char *, 
			unsigned int, 
			CUjit_option *, 
			void **)
			) dlsym(original_libcuda_handle, "cuLinkAddData_v2");
			fprintf(stderr, "original_cuLinkAddData_v2:%p\n", original_cuLinkAddData_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLinkAddData_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLinkAddData_v2(
		state, 
		type, 
		data, 
		size, 
		name, 
		numOptions, 
		options, 
		optionValues
		);
	}
}