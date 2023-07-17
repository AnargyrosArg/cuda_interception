#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcVersion)(
int *, 
int *
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcVersion(int *major, int *minor) {
		fprintf(stderr, "nvrtcVersion()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcVersion)
		{
			//fetch the original function addr using dlsym
			original_nvrtcVersion = (nvrtcResult (*)(
			int *, 
			int *)
			) dlsym(original_libnvrtc_handle, "nvrtcVersion");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcVersion():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcVersion(
		major, 
		minor
		);
	}
}