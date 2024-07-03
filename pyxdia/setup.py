#!/usr/bin/env python3

import platform
from pathlib import Path
from setuptools import setup
from setuptools.command.build_py import build_py
from setuptools.command.develop import develop as _develop
from wheel.bdist_wheel import bdist_wheel


class CustomBdistWheel(bdist_wheel):
    def get_tag(self):
        plat_to_tag = {"Windows": "win", "Linux": "manylinux_2_28", "Darwin": "macosx_11_0"}
        plat = plat_to_tag.get(platform.system(), None)
        if plat is None:
            raise RuntimeError("Unhandled platform tag")
        return ("py3", "none", f"{plat}_{platform.machine()}")


def build_common():
    root_dir = Path(__file__).parent.absolute()
    pyxdia_dir = root_dir / "pyxdia"
    bin_dir = pyxdia_dir / "bin"

    xdia_exe_path = bin_dir / "xdia.exe"
    msdia140_dll_path = bin_dir / "msdia140.dll"

    required_files = [xdia_exe_path, msdia140_dll_path]
    if platform.system() != "Windows":
        required_files.append(bin_dir / "xdialdr")
        if platform.system() != "Linux" or platform.machine() != "x86_64":
            required_files.append(bin_dir / "blink")

    for required_file in required_files:
        if not required_file.exists():
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
    cmdclass=dict(build_py=build, develop=develop, bdist_wheel=CustomBdistWheel),
    packages=["pyxdia"],
    package_data={"pyxdia": ["bin/*"]},
)
