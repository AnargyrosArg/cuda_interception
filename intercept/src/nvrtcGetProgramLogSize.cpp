#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcGetProgramLogSize)(
nvrtcProgram, 
size_t *
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcGetProgramLogSize(nvrtcProgram prog, size_t *logSizeRet) {
		fprintf(stderr, "nvrtcGetProgramLogSize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcGetProgramLogSize)
		{
			//fetch the original function addr using dlsym
			original_nvrtcGetProgramLogSize = (nvrtcResult (*)(
			nvrtcProgram, 
			size_t *)
			) dlsym(original_libnvrtc_handle, "nvrtcGetProgramLogSize");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcGetProgramLogSize():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcGetProgramLogSize(
		prog, 
		logSizeRet
		);
	}
}