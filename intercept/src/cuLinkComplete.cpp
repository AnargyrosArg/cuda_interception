#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLinkComplete)(
CUlinkState, 
void **, 
size_t *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLinkComplete(CUlinkState state, void **cubinOut, size_t *sizeOut) {
		fprintf(stderr, "cuLinkComplete()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLinkComplete)
		{
			//fetch the original function addr using dlsym
			original_cuLinkComplete = (CUresult (*)(
			CUlinkState, 
			void **, 
			size_t *)
			) dlsym(original_libcuda_handle, "cuLinkComplete");
			fprintf(stderr, "original_cuLinkComplete:%p\n", original_cuLinkComplete);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLinkComplete():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLinkComplete(
		state, 
		cubinOut, 
		sizeOut
		);
	}
}