pyxdia
======
pyxdia is a Python library for extracting useful program information from a [PDB file](https://en.wikipedia.org/wiki/Program_database). pyxdia runs on Windows, macOS, Linux, and other platforms.

## Dependencies

pyxdia depends on [xdia](https://github.com/mborgerson/xdia), a native Windows executable. For cross-platform support, a thin loader and compatibility layer (xdialdr) and emulator ([Blink](https://github.com/jart/blink)) are used to run xdia. These dependencies are included in wheels for convenient installation, and in source distributions will be downloaded at installation time.

## License

pyxdia source is released under MIT license. pyxdia wheels include binaries that are released under individual licenses, with license text available within the wheel.

## Installation

Install pyxdia with:

```
pip install pyxdia
```

## Usage

Read globals:

```
In [1]: from pyxdia import PDB

In [2]: pdb = PDB("./hello-wdm.pdb")

In [3]: pdb.globals[0]
Out[3]:
{'addressOffset': 48,
 'addressSection': 1,
 'name': '_guard_dispatch_icall_nop',
 'relativeVirtualAddress': 4144,
 'symTag': 'Function',
 'undecoratedName': '_guard_dispatch_icall_nop'}
```
