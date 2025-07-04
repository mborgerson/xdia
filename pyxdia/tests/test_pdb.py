#!/usr/bin/env python3
# pylint:disable=no-self-use

import os
import unittest

from pyxdia import PDB


PDB_TEST_FILES = os.getenv("PDB_TEST_FILES")
assert PDB_TEST_FILES is not None, "Set environment variable PDB_TEST_FILES"


class TestPDB(unittest.TestCase):
    """
    Test PDB class.
    """

    def test_instantiate(self):
        PDB(os.path.join(PDB_TEST_FILES, "hello-wdm.pdb"))

    def test_list_globals(self):
        pdb = PDB(os.path.join(PDB_TEST_FILES, "hello-wdm.pdb"))
        assert len(pdb.globals) == 78

        globals_by_name = {g["name"]: g for g in pdb.globals}
        assert globals_by_name["DriverEntry"] == {
            "addressOffset": 0,
            "addressSection": 5,
            "name": "DriverEntry",
            "relativeVirtualAddress": 20480,
            "symTag": "Function",
            "undecoratedName": "DriverEntry",
        }

    def test_list_publics(self):
        pdb = PDB(os.path.join(PDB_TEST_FILES, "hello-wdm.pdb"))
        assert len(pdb.publics) == 31

        publics_by_name = {g["name"]: g for g in pdb.publics}
        assert publics_by_name["DbgPrint"] == {
            "addressOffset": 0,
            "addressSection": 1,
            "is_function": True,
            "name": "DbgPrint",
            "relativeVirtualAddress": 4096,
            "symTag": "PublicSymbol",
            "undecoratedName": "DbgPrint",
        }


if __name__ == "__main__":
    unittest.main()
