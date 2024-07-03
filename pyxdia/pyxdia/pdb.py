import json
import os
import pathlib
import platform
import subprocess


ROOT_PATH = pathlib.Path(__file__).parent.absolute()
XDIA_EXE_PATH = ROOT_PATH / "bin" / "xdia.exe"
MSDIA_DLL_PATH = ROOT_PATH / "bin" / "msdia140.dll"
XDIALDR_PATH = ROOT_PATH / "bin" / "xdialdr"
BLINK_PATH = ROOT_PATH / "bin" / "blink"


def load_pdb(pdb_path: str | pathlib.Path):
	if not isinstance(pdb_path, pathlib.Path):
		pdb_path = pathlib.Path(pdb_path).absolute()

	env = {"MSDIA_PATH": str(MSDIA_DLL_PATH), "XDIA_PATH": str(XDIA_EXE_PATH)}
	blink_required = False

	if platform.system() == "Windows":
		cmd = [XDIA_EXE_PATH]
	else:
		# FIXME: Move to post-install hook
		for path in [XDIALDR_PATH, BLINK_PATH]:
			if path.exists():
				os.chmod(str(path), 0o755)

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

	cmd.append(str(pdb_path))

	diadump_s = subprocess.check_output(cmd, encoding="utf-8", env=env)
	return json.loads(diadump_s)
