# F2PY-build-script

## Prerequisites
- Linux system
- make
- awk
- Fortran compiler (e.g. `gfortran`)
- Python
- f2py
- [recommended] meson (if f2py version >= 1.26)
- [optional] Fortran preprocessor

If you use conda, f2py and meson can be installed by the following command:
```
$ conda install f2py meson
```

## Installation
1. Place the scripts in `f2py_build_script` on any directory
(e.g. `$HOME/local/make_build_script`).
2. Register the directory name in the environment variable `${F2PY_BUILD_SCRIPT}`.

```
📂${F2PY_BUILD_SCRIPT}
├──📄Makefile.example
├──📄f2py_f2cmap
├──📂gd
│   ├──📄gen-deps.awk
│   └──📄gen-inc-deps.awk
├──📄make.inc
└──📂simple
    ├──📄Makefile.example
    └──📄make.inc
```

## Usage
Copy `${F2PY_BUILD_SCRIPT}/Makefile.example` to your Fortran project directory as a file named `Makefile`.
You can also use the Makefile of the simple version `${F2PY_BUILD_SCRIPT}/simple/Makefile.example`.

## Example
test files: `tests/`

build (normal):
```
$ make -j
```

Building flags can be changed by editing the Makefile or by adding an option to the command as follows:
```
$ make DFLAGS=-D_INTLONG_ERROR -j
```


build (simple):
```
$ make -f Makefile-simple -j
```

run f90 script:
```
$ ./build/bin/calc_pi_test.x
                   10   3.2323158094055939        2.8878077401961865E-002
                  100   3.1514934010709905        3.1515058038743974E-003
                 1000   3.1425916543395429        3.1799181495037720E-004
                10000   3.1416926435905430        3.1827805758216028E-005
               100000   3.1416026534897892        3.1830670295952861E-006
              1000000   3.1415936535887807        3.1830956393207569E-007
             10000000   3.1415927535897326        3.1830969341599073E-008
            100000000   3.1415926635896865        3.1830649165761137E-009
           1000000000   3.1415926545896715        3.1827118043274782E-010
```

run python script (when `DFLAGS = -D_INTLONG_ERROR`):
```
$ ./calc_pi.py

python: nmax = 100
                  100   3.1514934010709905        3.1515058038743974E-003

python: nmax = 1000
                 1000   3.1425916543395429        3.1799181495037720E-004

python: nmax = 10000
                10000   3.1416926435905430        3.1827805758216028E-005

python: nmax = 100000
               100000   3.1416026534897896        3.1830670297366440E-006

python: nmax = 1000000
              1000000   3.1415936535887807        3.1830956393207569E-007

python: nmax = 10000000
             10000000   3.1415927535897326        3.1830969341599073E-008

python: nmax = 100000000
            100000000   3.1415926635896865        3.1830649165761137E-009

python: nmax = 1000000000
           1000000000   3.1415926545896720        3.1827132179073365E-010

python: nmax = 10000000000
 nmax exceeds maximum of 4 byte integer
```
