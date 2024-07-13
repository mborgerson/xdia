#!/bin/bash

set -e

if [ "$#" -ne 1 ]; then
    echo "usage: $0 version-number"
    exit 1
fi

VERSION=$1

if [[ ! "$VERSION" =~ ^([0-9]+)(\.[0-9]+)*(\.dev[0-9]+)?$ ]]; then
	echo "invalid version '$VERSION'! expected e.g. 0.1.2 or 0.1.2.dev0"
	exit 1
fi

echo "__version__ = \"$VERSION\"" > pyxdia/pyxdia/__version__.py

git add pyxdia/pyxdia/__version__.py
git commit -m v$VERSION
PUSHREFS="main"

if [[ ! "$VERSION" =~ "dev" ]]; then
	git tag -a v$VERSION -m v$VERSION
	PUSHREFS="$PUSHREFS v$VERSION"
fi

git push origin $PUSHREFS
