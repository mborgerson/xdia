xdia
====

xdia is a utility to extract useful program information from a [PDB file](https://en.wikipedia.org/wiki/Program_database) in an easily parsed format (currently JSON) for other tools to consume. xdia can be run on Windows, Linux, macOS, and other platforms.

A number of third party resources exist for parsing PDB files, however most have limitations in compatibility. This is due in large part to the fact that the PDB format has been around for a long time, is complex, apart from [some reference code](https://github.com/microsoft/microsoft-pdb) is only unofficially documented, and still evolving.

For superior compatibility, and to avoid maintaining yet another PDB parsing library, xdia simply leverages the official [Debug Interface Access SDK](https://learn.microsoft.com/en-us/visualstudio/debugger/debug-interface-access/debug-interface-access-sdk?view=vs-2022).

> [!NOTE]  
> xdia is a new project. Functionality and interface are expected to evolve.

## Cross-platform Support

The main application, `xdia.exe`, is a native Windows command-line application, but the project also includes `xdialdr` (xdia loader) which can load and run `xdia.exe` on Linux. `xdialdr` is a minimal loader and compatibility layer, like [Wine](https://www.winehq.org/) but focused only on running `xdia.exe` and its dependencies.

In combination with `xdialdr`, the [Blink](https://github.com/jart/blink) emulator can be used to run `xdia.exe` on other operating systems and architectures, including macOS on Apple Silicon.

## Usage

`xdia.exe /path/to/your.pdb`

or, if using xdialdr:

`xdialdr /path/to/your.pdb`

### Environment variables

| Name         | Description                                  |
|--------------|----------------------------------------------|
| `MSDIA_PATH` | Path to msdia140.dll, default ./msdia140.dll |
| `XDIA_PATH`  | Path to xdia.exe, default ./xdia.exe         |

## Limitations (What's extracted from the PDB?)

Currently, only globals are listed. Example:

```
> xdia.exe hello-wdm.pdb
{
  "globals": [
    {
      "addressOffset": 48,
      "addressSection": 1,
      "name": "_guard_dispatch_icall_nop",
      "relativeVirtualAddress": 4144,
      "symTag": "Function",
      "undecoratedName": "_guard_dispatch_icall_nop"
    },
...
}
```

It is a near-term goal of xdia project to provide more information, namely detailed type information.

## Building xdia

`xdia.exe` is intended only to be built with Visual Studio tools targeting x86-64 and CMake:

```
cmake -Ssrc/xdia -Bbuild -Ax64
cmake --build build --config Release --parallel --verbose
cmake --install build --prefix dist
cd dist
xdia.exe <path-to-pdb>
```

## Building xdialdr

Similarly, `xdialdr` is intended to be built targeting an x86-64 Linux machine with CMake:

```
cmake -S. -Bbuild
cmake --build build --target xdialdr
cmake --install build --prefix dist
cd dist
./xdialdr <path-to-pdb>
```

## Python Support

pyxdia provides a Python interface to xdia. PyPI release coming shortly.

## License

- [xdia](src/xdia/docs/xdia.LICENSE.txt)
- [xdialdr](src/xdia-loader/docs/xdialdr.LICENSE.txt)
- [pyxdia](pyxdia/LICENSE.txt)
