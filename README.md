# Parallelization APIs. Travelling salesman problem.

- [Abstract](abstract)
- [Report](Comparison-of-parallelization-APIs-Travelling-salesman-problem.pdf)

## Requirements

- languages: C++17, C11
- compiler: [GCC 7.3.0](https://ftp.gnu.org/gnu/gcc/gcc-7.3.0/)
- build config: [CMake 3.9](https://cmake.org/cmake/help/v3.9/)
- environment: [Python 3.6.7](https://www.python.org/downloads/release/python-367/)
- APIs: [OpenMP](https://www.openmp.org/wp-content/uploads/OpenMP3.0-SummarySpec.pdf), [Pthreads](http://man7.org/linux/man-pages/man7/pthreads.7.html), [std::thread](http://www.cplusplus.com/reference/thread/thread/), [OpenMPI](https://www.open-mpi.org/)

## How to use
1. Build project using makefiles created with help of CMake and source files so as to have the executables
     in folders `cmake-build-debug` and `cmake-build-release` You can simply do that e.g. using CLion or try it manually.
2. Profile solver using `profile.py`.
3. Generate CSV using `report.py`.

You can run scripts with argument `-h` to see help.

**Note**: `profile.py` and `report.py` are written to be run from the root project directory. You can change the
project or target name in CMake or runner script, but then you need to accustom the flow at your own risk.

**Manual compilation**

```shell
$ mkdir cmake-build-debug; cd cmake-build-debug
$ cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..
$ cmake --build --target all -- -j 4; cd ..
$ mkdir cmake-build-release; cd cmake-build-release
$ cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - Unix Makefiles" ..
$ cmake --build --target all -- -j 4; cd ..
```

**Profiler**

_Debug (verbose) OpenMP C++ solver 6 times using 2 threads to solve TSP for an automatically-generated graph of
size 10, append a time which algorithm spend on calculation to `tests/CPP_OpenMPSolver_size_10_slots_02.txt`_

```shell
$ python profile.py -m debug --lang cpp -i 10 --type omp --slots 2 --verbose
```

**Generating CSV**

_Generate CSV from `tests/` to `summary.csv`_

```shell
$ python report.py --folder tests --output summary.csv
```

---

*Enjoy your code-meal!*

*marcullo*
