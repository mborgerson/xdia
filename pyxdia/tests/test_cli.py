#!/usr/bin/env python3
# pylint:disable=no-self-use

import os
import subprocess
import sys
import unittest

from pyxdia import PDB


PDB_TEST_FILES = os.getenv("PDB_TEST_FILES")
assert PDB_TEST_FILES is not None, "Set environment variable PDB_TEST_FILES"


class TestCli(unittest.TestCase):
    """
    Test pyxdia module command line interface
    """

    def test_cli(self):
        subprocess.run([sys.executable, "-m", "pyxdia", os.path.join(PDB_TEST_FILES, "hello-wdm.pdb")], check=True)


if __name__ == "__main__":
    unittest.main()
