#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcGetNVVM)(
nvrtcProgram, 
char *
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcGetNVVM(nvrtcProgram prog, char *nvvm) {
		fprintf(stderr, "nvrtcGetNVVM()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcGetNVVM)
		{
			//fetch the original function addr using dlsym
			original_nvrtcGetNVVM = (nvrtcResult (*)(
			nvrtcProgram, 
			char *)
			) dlsym(original_libnvrtc_handle, "nvrtcGetNVVM");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcGetNVVM():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcGetNVVM(
		prog, 
		nvvm
		);
	}
}