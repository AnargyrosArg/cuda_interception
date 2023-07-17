#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcGetCUBIN)(
nvrtcProgram, 
char *
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcGetCUBIN(nvrtcProgram prog, char *cubin) {
		fprintf(stderr, "nvrtcGetCUBIN()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcGetCUBIN)
		{
			//fetch the original function addr using dlsym
			original_nvrtcGetCUBIN = (nvrtcResult (*)(
			nvrtcProgram, 
			char *)
			) dlsym(original_libnvrtc_handle, "nvrtcGetCUBIN");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcGetCUBIN():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcGetCUBIN(
		prog, 
		cubin
		);
	}
}