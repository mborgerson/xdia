import subprocess
import pathlib
import platform
import json

ROOT_PATH = pathlib.Path(__file__).parent
XDIA_EXE_PATH = ROOT_PATH / "bin" / "xdia.exe"
MSDIA_DLL_PATH = ROOT_PATH / "bin" / "msdia140.dll"

if platform.system() in ("Linux", "Darwin"):
	XDIALDR_PATH = ROOT_PATH / "bin" / "xdialdr"
if platform.system() == "Darwin":
	BLINK_PATH = ROOT_PATH / "bin" / "blink"

def load_pdb(pdb_path: pathlib.Path):
	env = {"MSDIA_PATH": str(MSDIA_DLL_PATH), "XDIA_PATH": str(XDIA_EXE_PATH)}

	if platform.system() == "Windows":
		cmd = [XDIA_EXE_PATH]
	elif platform.system() == "Linux":
		cmd = [XDIALDR_PATH]
	elif platform.system() == "Darwin":
		cmd = [BLINK_PATH, XDIALDR_PATH]
	else:
		raise NotImplementedError("Unhandled platform")

	cmd += [pdb_path]

	diadump_s = subprocess.check_output(cmd, encoding="utf-8", env=env)
	return json.loads(diadump_s)
