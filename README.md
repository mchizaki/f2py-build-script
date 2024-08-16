# F2PY-build-script

## Prerequisites
- Linux system
- make
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
1. Download these script files and place them in any directory
(e.g. `$HOME/local/make_build/script`).
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

