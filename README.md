# Competitive Programming Tester
Tool for testing your algorithms
## Getting Started

Now **cpt** is presented as a command-line tool as **cptc**, but the web-based version is also an option in the future.
### Prerequisites

Project is built by **make** which uses **gcc** (or **clang** for Mac) for compiling. Currently **Makefile** uses **POSIX**-compliant commands, so it is not tested on **Windows**.
### Installing

As it was mentioned **cpt** uses **make** as a build system. And the core **libcpt** library is used in a command-line tool or a web-based application.

To build the library simply run

```
make cpt
```

Or to build command-line tool

```
make cptc
```

All the binaries and object files are now in the **build** directory.