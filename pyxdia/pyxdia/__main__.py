import argparse
from pprint import pprint

from .pdb import load_pdb


def main():
	ap = argparse.ArgumentParser("pyxdia")
	ap.add_argument("pdb", help="Path to PDB file")
	args = ap.parse_args()

	pdb = load_pdb(args.pdb)
	pprint(pdb, indent=2)


if __name__ == '__main__':
	main()
