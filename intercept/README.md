# Interception Library
Compile with
    make -j

Creates a dynamic library intended to be preloaded into an executable with LD_PRELOAD.
By default it only intercepts calls to cudart and cuda API and prints a trace.