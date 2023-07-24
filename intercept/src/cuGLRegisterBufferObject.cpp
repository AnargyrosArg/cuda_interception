#include <cudaGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGLRegisterBufferObject)(
GLuint
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGLRegisterBufferObject(GLuint buffer) {
		fprintf(stderr, "cuGLRegisterBufferObject()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGLRegisterBufferObject)
		{
			//fetch the original function addr using dlsym
			original_cuGLRegisterBufferObject = (CUresult (*)(
			GLuint)
			) dlsym(original_libcuda_handle, "cuGLRegisterBufferObject");
			fprintf(stderr, "original_cuGLRegisterBufferObject:%p\n", original_cuGLRegisterBufferObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGLRegisterBufferObject():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGLRegisterBufferObject(
		buffer
		);
	}
}