#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
#include <string.h>

CUresult (*original_cuGetProcAddress)(const char *, void **, int, cuuint64_t);

//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern void* libintercept_handle;
extern "C"
{
	CUresult cuGetProcAddress(const char *symbol, void **pfn, int cudaVersion, cuuint64_t flags) {
		char* __dlerror;
		fprintf(stderr, "cuGetProcAddress() called for symbol %s\n",symbol);
		//return CUDA_ERROR_NOT_FOUND;
		//this call clears any previous errors
		dlerror();
		//	this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGetProcAddress){
			//fetch the original function addr using dlsym
			original_cuGetProcAddress = (CUresult (*)(const char *, void **, int, cuuint64_t)) dlsym(original_libcuda_handle, "cuGetProcAddress");
			fprintf(stderr, "original cuGetProcAddress address as returned by dlsym:%p\n", original_cuGetProcAddress);
		}
		__dlerror = dlerror();
		if(__dlerror) fprintf(stderr, "dlsym error for function cuGetProcAddress():%s\n", __dlerror);

		//CUresult res =  original_cuGetProcAddress(symbol, pfn, cudaVersion, flags);
		//return res;
		CUresult res = CUDA_SUCCESS;
		void* handle;
		if(libintercept_handle ==NULL){
			handle = dlopen("libintercept.so",RTLD_NOW);
			libintercept_handle = handle;
		}else{
			handle = libintercept_handle;
		}
		__dlerror = dlerror();
		if(__dlerror) fprintf(stderr, "dlopen:%s\n", __dlerror);
		
		// 2000-3010 3020-11030 11040+
		if(!strcmp(symbol,"cuCtxCreate")){
			if(cudaVersion >= 2000 && cudaVersion <= 3010){
				*pfn = dlsym(handle,"cuCtxCreate");
			}else if(cudaVersion >= 3020 && cudaVersion <= 11030){
				*pfn = dlsym(handle,"cuCtxCreate_v2");
			}else if(cudaVersion >= 11040){
				*pfn = dlsym(handle,"cuCtxCreate_v3");
			}else{
				pfn = NULL;
				return CUDA_ERROR_NOT_FOUND;
			}
			return CUDA_SUCCESS;
		}

		*pfn = dlsym(handle,symbol);
		
		__dlerror = dlerror();
		if(__dlerror){
			char symbol2[strlen(symbol)+4];
			for(int i=0;i<strlen(symbol);i++){
				symbol2[i] = symbol[i];
			}
			symbol2[strlen(symbol)] = '_';
			symbol2[strlen(symbol)+1] = 'v';
			symbol2[strlen(symbol)+2] = '2';
			symbol2[strlen(symbol)+3] = '\0';
			*pfn = dlsym(handle,symbol2);
			
			__dlerror = dlerror();
			if(__dlerror){
				symbol2[strlen(symbol)+2] = '3';
				*pfn = dlsym(handle,symbol2);
			}

			__dlerror = dlerror();
			if(__dlerror){
				fprintf(stderr,"dlsym:%s\n",__dlerror);
				res = CUDA_ERROR_NOT_FOUND;
			}
			return res;
		}
		return res;
	}
}