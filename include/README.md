# Why a bref/ subdirectory ?

We choose to put the bref API header files in the `bref/`
subdirectory, so classes in the namespace `::bref` will be included by
`#include "bref/...`.

    #include "bref/Pipeline.h"

    // ...
    bref::Pipeline pipeline;
