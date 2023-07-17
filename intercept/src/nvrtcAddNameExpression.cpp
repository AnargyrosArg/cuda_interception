#include <nvrtc.h>
#include <dlfcn.h>
#include <iostream>
nvrtcResult (*original_nvrtcAddNameExpression)(
nvrtcProgram, 
const char * const
);
//handle to the actual libnvrtc library, used to fetch original functions with dlsym
extern void* original_libnvrtc_handle;
extern "C"
{
	nvrtcResult nvrtcAddNameExpression(nvrtcProgram prog, const char * const name_expression) {
		fprintf(stderr, "nvrtcAddNameExpression()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libnvrtc (original_libnvrtc_handle)
		if(original_libnvrtc_handle == NULL){
			dlopen("libnvrtc.so.11.2", RTLD_NOW);
		}
		if (!original_nvrtcAddNameExpression)
		{
			//fetch the original function addr using dlsym
			original_nvrtcAddNameExpression = (nvrtcResult (*)(
			nvrtcProgram, 
			const char * const)
			) dlsym(original_libnvrtc_handle, "nvrtcAddNameExpression");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function nvrtcAddNameExpression():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_nvrtcAddNameExpression(
		prog, 
		name_expression
		);
	}
}