#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcGetProgramLog)(
nvrtcProgram, 
char *
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcGetProgramLog(nvrtcProgram prog, char *log) {
		fprintf(stderr, "nvrtcGetProgramLog()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcGetProgramLog)
		{
			//fetch the original function addr using dlsym
			original_nvrtcGetProgramLog = (nvrtcResult (*)(
			nvrtcProgram, 
			char *)
			) dlsym(original_libnvrtc_handle, "nvrtcGetProgramLog");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcGetProgramLog():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcGetProgramLog(
		prog, 
		log
		);
	}
}