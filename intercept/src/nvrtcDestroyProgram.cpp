#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcDestroyProgram)(
nvrtcProgram *
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcDestroyProgram(nvrtcProgram *prog) {
		fprintf(stderr, "nvrtcDestroyProgram()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcDestroyProgram)
		{
			//fetch the original function addr using dlsym
			original_nvrtcDestroyProgram = (nvrtcResult (*)(
			nvrtcProgram *)
			) dlsym(original_libnvrtc_handle, "nvrtcDestroyProgram");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcDestroyProgram():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcDestroyProgram(
		prog
		);
	}
}