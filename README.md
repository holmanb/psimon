# psimon

monitoring utility for pressure stall information

## Runtime Requirements

Kernel must support psi (`CONFIG_PSI=y`), which requires at least linux 4.20 - ![Read the Docs](https://www.kernel.org/doc/html/latest/accounting/psi.html)

## Build Requirements

`git`
`gcc`
`make`


## Build Instructions

```
git clone --recurse-submodules https://github.com/holmanb/psimon.git
cd psimon
make
make psimon
```



