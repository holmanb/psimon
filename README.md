# psimon

monitoring utility for pressure stall information


# What is Pressure Stall Information?

> The “some” line indicates the share of time in which at least some tasks are stalled on a given resource.
>
> The “full” line indicates the share of time in which all non-idle tasks are stalled on a given resource simultaneously. In this state actual CPU cycles are going to waste, and a workload that spends extended time in this state is considered to be thrashing. This has severe impact on performance, and it’s useful to distinguish this situation from a state where some tasks are stalled but the CPU is still doing productive work. As such, time spent in this subset of the stall state is tracked separately and exported in the “full” averages.

Want to know more? You can read more on ![kernel.org](https://www.kernel.org/doc/html/latest/accounting/psi.html)


## Runtime Requirements

Kernel must support psi (`CONFIG_PSI=y`), which requires at least linux 4.20


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



