#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
struct cudaChannelFormatDesc (*original_cudaCreateChannelDesc)(
int, 
int, 
int, 
int, 
enum cudaChannelFormatKind
);
extern "C"
{
	struct cudaChannelFormatDesc cudaCreateChannelDesc(int x, int y, int z, int w, enum cudaChannelFormatKind f) {
		fprintf(stderr, "===============\ncudaCreateChannelDesc()\n");
		char* __dlerror;
		if (!original_cudaCreateChannelDesc)
		{
			//fetch the original function addr using dlsym
			original_cudaCreateChannelDesc = (struct cudaChannelFormatDesc (*)(
			int, 
			int, 
			int, 
			int, 
			enum cudaChannelFormatKind)
			) dlsym(RTLD_NEXT, "cudaCreateChannelDesc");
			fprintf(stderr, "original_cudaCreateChannelDesc:%p\n", original_cudaCreateChannelDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaCreateChannelDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		struct cudaChannelFormatDesc retval = original_cudaCreateChannelDesc(
		x, 
		y, 
		z, 
		w, 
		f
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}