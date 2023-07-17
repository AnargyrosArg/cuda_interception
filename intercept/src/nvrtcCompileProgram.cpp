#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcCompileProgram)(
nvrtcProgram, 
int, 
const char * const*
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcCompileProgram(nvrtcProgram prog, int numOptions, const char * const *options) {
		fprintf(stderr, "nvrtcCompileProgram()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcCompileProgram)
		{
			//fetch the original function addr using dlsym
			original_nvrtcCompileProgram = (nvrtcResult (*)(
			nvrtcProgram, 
			int, 
			const char * const*)
			) dlsym(original_libnvrtc_handle, "nvrtcCompileProgram");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcCompileProgram():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcCompileProgram(
		prog, 
		numOptions, 
		options
		);
	}
}