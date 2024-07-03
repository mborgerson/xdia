#!/usr/bin/env python3

import gzip
import shutil
import platform
import os
import stat
from zipfile import ZipFile
from urllib.request import urlretrieve, urlcleanup
from pathlib import Path
from setuptools import setup, Command
from setuptools.command.build import build as _build
from setuptools.command.build import SubCommand
from setuptools.command.install import install as _install
from setuptools.command.develop import develop as _develop
from wheel.bdist_wheel import bdist_wheel


root_dir = Path(__file__).parent.absolute()
pyxdia_dir = root_dir / "pyxdia"


with open(pyxdia_dir / "__version__.py") as f:
    ns = {}
    exec(f.read(), ns)
    version = ns["__version__"]


class CustomBdistWheel(bdist_wheel):
    """
    Overrides bdist_wheel to generate platform tag marking the wheel as
    platform specific.
    """

    def get_tag(self):
        plat_to_tag = {"Windows": "win", "Linux": "linux", "Darwin": "macosx_14_0"}
        plat = plat_to_tag.get(platform.system(), None)
        assert plat is not None, "FIXME: Unhandled platform tag"
        return ("py3", "none", f"{plat}_{platform.machine().lower()}")


class CheckXdiaInstallation(Command):
    """
    Check for xdia, xdialdr, blink installation. For installations from
    source, download and install necessary components if they are not already
    installed.
    """

    description = 'Check xdia installation'
    user_options = []
    editable_mode: bool = False

    def initialize_options(self):
        pass

    def finalize_options(self):
        if self.editable_mode:
            prefix = pyxdia_dir
        else:
            build_py = self.get_finalized_command('build_py')
            prefix = Path(build_py.build_lib) / "pyxdia"

        self._bin_dir = prefix / "bin"
        self._xdia_exe_path = self._bin_dir / "xdia.exe"
        self._xdialdr_path = self._bin_dir / "xdialdr"
        self._msdia140_dll_path = self._bin_dir / "msdia140.dll"
        self._blink_path = self._bin_dir / "blink"

    @staticmethod
    def _download_file(url) -> str:
        print(f'Downloading {url}...')
        filename = url.split("/")[-1]
        urlretrieve(url, filename)
        urlcleanup()
        return filename

    @staticmethod
    def _mark_file_executable(path):
        os.chmod(path, 0o755)

    def _get_xdia_install_list(self) -> list[str]:
        files = [self._xdia_exe_path, self._msdia140_dll_path]
        if platform.system() != "Windows":
            files.append(self._xdialdr_path)
        return files

    def _is_xdia_installed(self) -> bool:
        return all(file.exists() for file in self._get_xdia_install_list())

    def _install_xdia(self):
        if ".dev" in version:
            # FIXME: On Windows, run cmake to build xdia
            raise RuntimeError("xdia not available for dev version. Please build and install xdia manually.")

        xdia_release_url = "https://github.com/mborgerson/xdia/releases/download"

        # Install xdia
        path = self._download_file(f"{xdia_release_url}/v{version}/xdia.zip")
        with ZipFile(path) as z:
            z.extractall(self._bin_dir)

        # Install xdialdr, if required
        if platform.system() != "Windows":
            path = self._download_file(f"{xdia_release_url}/v{version}/xdialdr.zip")
            with ZipFile(path) as z:
                z.extractall(self._bin_dir)
            self._mark_file_executable(self._xdialdr_path)

    @staticmethod
    def _is_blink_required() -> bool:
        return platform.system() != "Windows" and (platform.system() != "Linux" or platform.machine() != "x86_64")

    def _get_blink_install_list(self) -> list[str]:
        return [self._blink_path]

    def _is_blink_installed(self) -> bool:
        return all(file.exists() for file in self._get_blink_install_list())

    @staticmethod
    def gunzip(src, dst):
        with gzip.open(src, "rb") as s_file, open(dst, "wb") as d_file:
            shutil.copyfileobj(s_file, d_file, 65536)

    def _install_blink(self):
        blink_version = "1.1.0"
        blink_release_platform_filename_parts = {
            "darwin-arm64": "darwin-arm64.gz",  # ???
            "darwin-x86_64": "darwin-x86_64.elf",  # ???
            "freebsd-x86_64": "freebsd-x86_64.elf.gz",
            "linux-aarch64": "linux-aarch64.elf.gz",
            "linux-arm": "linux-arm.elf.gz",
            "linux-i486": "linux-i486.elf.gz",
            "linux-mips": "linux-mips.elf.gz",
            "linux-mips64": "linux-mips64.elf.gz",
            "linux-mips64el": "linux-mips64el.elf.gz",
            "linux-mipsel": "linux-mipsel.elf.gz",
            "linux-powerpc": "linux-powerpc.elf.gz",
            "linux-powerpc64le": "linux-powerpc64le.elf.gz",
            "linux-s390x": "linux-s390x.elf.gz",
            "linux-x86_64": "linux-x86_64.elf.gz",
            "openbsd-x86_64": "openbsd-x86_64.elf.gz",
        }
        tag = f"{platform.system().lower()}-{platform.machine().lower()}"
        blink_platform = blink_release_platform_filename_parts.get(tag, None)
        assert blink_platform is not None, "FIXME: Unhandled platform tag"

        blink_filename = f"blink-{blink_version}-{blink_platform}"
        blink_release_url = f"https://github.com/jart/blink/releases/download/{blink_version}/{blink_filename}"

        self._download_file(blink_release_url)
        if blink_filename.endswith(".gz"):
            self.gunzip(blink_filename, self._blink_path)
        else:
            shutil.copyfile(blink_filename, self._blink_path)
        self._mark_file_executable(self._blink_path)

    def run(self):
        if not self._is_xdia_installed():
            self._install_xdia()

        if self._is_blink_required():
            if not self._is_blink_installed():
                self._install_blink()

    def get_outputs(self):
        outputs = self._get_xdia_install_list()
        if self._is_blink_required():
            outputs += self._get_blink_install_list()
        return outputs


class build(_build):
    sub_commands = _build.sub_commands + [('check_xdia_install', None)]


setup(
    cmdclass={
        'build': build,
        'check_xdia_install': CheckXdiaInstallation,
        'bdist_wheel': CustomBdistWheel
    },
    packages=["pyxdia"],
    package_data={"pyxdia": ["bin/*"]},
)
