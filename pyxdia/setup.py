#!/usr/bin/env python3

from pathlib import Path
import platform
import subprocess
from setuptools import setup
from setuptools.command.build_py import build_py
from setuptools.command.develop import develop as _develop

def build_common():
    root_dir = Path(__file__).parent.absolute()
    pyxdia_dir = root_dir / "pyxdia"
    bin_dir = pyxdia_dir / "bin"
    xdia_exe_path = bin_dir / "xdia.exe"
    msdia140_dll_path = bin_dir / "msdia140.dll"

    if not (xdia_exe_path.exists() and msdia140_dll_path.exists()):
        raise RuntimeError("Missing xdia installation")

class build(build_py):
    def run(self):
        build_common()
        return super().run()

class develop(_develop):
    def run(self):
        build_common()
        return super().run()

setup(
    cmdclass=dict(build_py=build, develop=develop),
    packages=['pyxdia'],
    package_data={'pyxdia': ['bin/*']},
    )
