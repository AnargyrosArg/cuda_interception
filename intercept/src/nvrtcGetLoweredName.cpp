#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcGetLoweredName)(
nvrtcProgram, 
const char *, 
const char **
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcGetLoweredName(nvrtcProgram prog, const char * const name_expression, const char **lowered_name) {
		fprintf(stderr, "nvrtcGetLoweredName()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcGetLoweredName)
		{
			//fetch the original function addr using dlsym
			original_nvrtcGetLoweredName = (nvrtcResult (*)(
			nvrtcProgram, 
			const char *, 
			const char **)
			) dlsym(original_libnvrtc_handle, "nvrtcGetLoweredName");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcGetLoweredName():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcGetLoweredName(
		prog, 
		name_expression, 
		lowered_name
		);
	}
}