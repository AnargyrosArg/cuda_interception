#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
const char * (*original_nvrtcGetErrorString)(
nvrtcResult
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	const char * nvrtcGetErrorString(nvrtcResult result) {
		fprintf(stderr, "nvrtcGetErrorString()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcGetErrorString)
		{
			//fetch the original function addr using dlsym
			original_nvrtcGetErrorString = (const char * (*)(
			nvrtcResult)
			) dlsym(original_libnvrtc_handle, "nvrtcGetErrorString");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcGetErrorString():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcGetErrorString(
		result
		);
	}
}