#!/bin/bash
# Fallback script when klipper-translations setup is incorrect and
# the deploy workflow failed. It only use the static files and only
# build the English site.
# Prepare English Fallback

MKDOCS_DIR="docs/_klipper3d/"

cat "${MKDOCS_DIR}base.yml" >> "${MKDOCS_DIR}en.yml"
cat "${MKDOCS_DIR}extra.yml" >> "${MKDOCS_DIR}en.yml"
cat "${MKDOCS_DIR}nav_en.yml" >> "${MKDOCS_DIR}en.yml"
mkdir docs/en
mv docs/*.md docs/en/
cp -r docs/img docs/en/img
cp -r docs/prints docs/en/prints
# generate fall back files
mkdocs build -f docs/_klipper3d/en.yml
#Set en to default
mv -f site/en/* site/
