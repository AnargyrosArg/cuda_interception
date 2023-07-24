#include <cudaGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGLSetBufferObjectMapFlags)(
GLuint, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGLSetBufferObjectMapFlags(GLuint buffer, unsigned int Flags) {
		fprintf(stderr, "cuGLSetBufferObjectMapFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGLSetBufferObjectMapFlags)
		{
			//fetch the original function addr using dlsym
			original_cuGLSetBufferObjectMapFlags = (CUresult (*)(
			GLuint, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGLSetBufferObjectMapFlags");
			fprintf(stderr, "original_cuGLSetBufferObjectMapFlags:%p\n", original_cuGLSetBufferObjectMapFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGLSetBufferObjectMapFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGLSetBufferObjectMapFlags(
		buffer, 
		Flags
		);
	}
}