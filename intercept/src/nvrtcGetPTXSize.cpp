#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcGetPTXSize)(
nvrtcProgram, 
size_t *
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcGetPTXSize(nvrtcProgram prog, size_t *ptxSizeRet) {
		fprintf(stderr, "nvrtcGetPTXSize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcGetPTXSize)
		{
			//fetch the original function addr using dlsym
			original_nvrtcGetPTXSize = (nvrtcResult (*)(
			nvrtcProgram, 
			size_t *)
			) dlsym(original_libnvrtc_handle, "nvrtcGetPTXSize");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcGetPTXSize():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcGetPTXSize(
		prog, 
		ptxSizeRet
		);
	}
}