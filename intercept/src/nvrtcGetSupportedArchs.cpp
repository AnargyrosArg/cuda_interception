#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcGetSupportedArchs)(
int *
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcGetSupportedArchs(int *supportedArchs) {
		fprintf(stderr, "nvrtcGetSupportedArchs()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcGetSupportedArchs)
		{
			//fetch the original function addr using dlsym
			original_nvrtcGetSupportedArchs = (nvrtcResult (*)(
			int *)
			) dlsym(original_libnvrtc_handle, "nvrtcGetSupportedArchs");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcGetSupportedArchs():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcGetSupportedArchs(
		supportedArchs
		);
	}
}