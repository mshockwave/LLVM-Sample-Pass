Simple LLVM Pass Demo
======================

TL;DR
------
This pass is a tutorial LLVM pass that store every binary operation results
into memory and load it back immediately.
 
Build
------
```
mkdir build
cd build
cmake -DLLVM_DIST_PATH=<Your LLVM install path> ..
make
```
The result pass module would be in `build/lib`

Usage
-----
```
opt -load build/lib/LLVMRedundantStore.[so|dylib|dll] \
    -redundant-store -S test.ll -o=output.ll
```
**Note: You need to use the `opt` in the same LLVM distribution 
you used to build this pass**