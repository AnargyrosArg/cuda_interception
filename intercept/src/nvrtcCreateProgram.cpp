#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcCreateProgram)(
nvrtcProgram *, 
const char *, 
const char *, 
int, 
const char * const*, 
const char * const*
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcCreateProgram(nvrtcProgram *prog, const char *src, const char *name, int numHeaders, const char * const *headers, const char * const *includeNames) {
		fprintf(stderr, "nvrtcCreateProgram()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcCreateProgram)
		{
			//fetch the original function addr using dlsym
			original_nvrtcCreateProgram = (nvrtcResult (*)(
			nvrtcProgram *, 
			const char *, 
			const char *, 
			int, 
			const char * const*, 
			const char * const*)
			) dlsym(original_libnvrtc_handle, "nvrtcCreateProgram");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcCreateProgram():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcCreateProgram(
		prog, 
		src, 
		name, 
		numHeaders, 
		headers, 
		includeNames
		);
	}
}