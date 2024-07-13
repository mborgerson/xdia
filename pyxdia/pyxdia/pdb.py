import json
import os
import pathlib
import platform
import subprocess
from pprint import pprint


ROOT_PATH = pathlib.Path(__file__).parent.absolute()
XDIA_EXE_PATH = ROOT_PATH / "bin" / "xdia.exe"
MSDIA_DLL_PATH = ROOT_PATH / "bin" / "msdia140.dll"
XDIALDR_PATH = ROOT_PATH / "bin" / "xdialdr"
BLINK_PATH = ROOT_PATH / "bin" / "blink"


class PDB:
    """
    This class interfaces with the xdia utility to extract useful program information from a Program Database (PDB) file.
    """

    _pdb_data: dict

    def __init__(self, pdb_path: str | pathlib.Path):
        self._load_pdb(pdb_path)

    def _load_pdb(self, pdb_path: str | pathlib.Path) -> None:
        # Ensure pdb_path is a pathlib.Path
        if not isinstance(pdb_path, pathlib.Path):
            pdb_path = pathlib.Path(pdb_path).absolute()

        env = {"MSDIA_PATH": str(MSDIA_DLL_PATH), "XDIA_PATH": str(XDIA_EXE_PATH)}

        if platform.system() == "Windows":
            cmd = [XDIA_EXE_PATH]
        else:
            blink_required = (platform.system() != "Linux") or (platform.machine() != "x86_64")
            if blink_required:
                # XXX: Work around blink VFS mount issue
                env["BLINK_PREFIX"] = "/tmp"
                env["MSDIA_PATH"] = "/SystemRoot" + env["MSDIA_PATH"]
                env["XDIA_PATH"] = "/SystemRoot" + env["XDIA_PATH"]
                pdb_path = pathlib.Path("/SystemRoot") / pdb_path.relative_to("/")
                xdialdr_path = pathlib.Path("/SystemRoot") / XDIALDR_PATH.relative_to("/")
                cmd = [BLINK_PATH, xdialdr_path]
            else:
                cmd = [XDIALDR_PATH]

        diadump_s = subprocess.check_output(cmd + [str(pdb_path)], encoding="utf-8", env=env)
        self._pdb_data = json.loads(diadump_s)

    def pp(self) -> None:
        """
        Pretty-print all available info.
        """
        pprint(self._pdb_data, indent=2)

    @property
    def globals(self) -> list[dict[str, int | str]]:
        """
        List all globals.
        """
        return self._pdb_data["globals"]
