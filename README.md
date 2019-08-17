# Competitive Programming Tester
Tool for testing your algorithms

![Alt Text](preview.gif)

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

## Command-line interface

Usage: **cptc** [OPTIONS] program

### Options

| Name        | Description                                                                    |
| ----------- | ------------------------------------------------------------------------------ |
| -h,--help   | Print this help message and exit                                               |
| -i,--input  | Input file (i.txt by default)                                                  |
| -o,--output | Output file (o.txt by default)                                                 |
| -d,--dir    | Directory with tests (current directory by default)                            |
| -n,--num    | Number of tests (if n > 1 then i[2..n].txt and o[2..n].txt files will be used) |
| -s,--silent | Do not print messages while testing                                            |
| -t,--tests  | Tests ranges divided by comma (a:b, n, ...)                                    |
| -e,--time   | Elapse test's time execution                                                   |

### Dependencies

- Header-only [CLI11](https://github.com/CLIUtils/CLI11) for arguments parsing


​              