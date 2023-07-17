#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcGetNumSupportedArchs)(
int *
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcGetNumSupportedArchs(int *numArchs) {
		fprintf(stderr, "nvrtcGetNumSupportedArchs()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcGetNumSupportedArchs)
		{
			//fetch the original function addr using dlsym
			original_nvrtcGetNumSupportedArchs = (nvrtcResult (*)(
			int *)
			) dlsym(original_libnvrtc_handle, "nvrtcGetNumSupportedArchs");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcGetNumSupportedArchs():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcGetNumSupportedArchs(
		numArchs
		);
	}
}