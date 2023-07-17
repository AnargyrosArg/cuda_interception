# Description
Interception of Cuda Driver API calls (libcuda.so) using LD_PRELOAD, dlopen() and dlsym()

# Compile

Go to /intercept and run 
    make -j

Then run with an executable LD_PRELOAD=/path/to/libintercept.so ./executable
The examples have a run.sh script or a Makefile run rule

# Note
Do not move libintercept.so or dlopen might not be able to find it during runtime.
If you must move it either modify the -Wl,-rpath,'/path/to/new/location' in /intercept/Makefile or set an appropriate LD_LIBRARY_PATH env var.