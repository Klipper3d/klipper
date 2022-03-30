#!/bin/bash
# Modify the file structure before running mkdocs
# This is a make shift multi-site building script before
# the current structure of Klipper-translations can be
# directly utilized by MkDocs.

MKDOCS_DIR="docs/_klipper3d/"

# fetch translations
git clone --depth 1 https://github.com/Klipper3d/klipper-translations

# move English Markdown Files
mkdir docs/en
cp docs/*.md docs/en/
cp -r docs/img docs/en/img
cp -r docs/prints docs/en/prints

# Move translation markdown files
while IFS="," read dirname langsite langdesc langsearch; do
  new_local_dir="docs/${langsite}"
  local_dir="klipper-translations/docs/locales/$dirname"
  mkdir "$new_local_dir"
  # move and rename markdown files
  echo "Moving $dirname to $langsite"
  mv "$local_dir"/*.md "$new_local_dir"

  echo "copy resources"
  cp -r docs/img "docs/${langsite}/img"
  cp -r docs/prints "docs/${langsite}/prints"

  # manually replace index.md if a manual-index.md exist
  manual_index="${new_local_dir}/manual-index.md"
  if [[ -f "$manual_index" ]];then
    mv -f "$manual_index" "${new_local_dir}/index.md"
    echo "replaced index.md with manual_index.md for $langsite"
  else
      echo "Manually translated index file for $langsite not found!"
  fi

  # Insert entries to extra.yml for language switching
  sed -i "s/.*#-# Alternate Entries For Translations/\
#-# Alternate Entries For Translations\n    - name: ${langdesc}\n      \
link: \/${langsite}\/\\n      lang: ${langsite}\\n/" "${MKDOCS_DIR}mkdocs.yml"

done <  <(egrep -v '^ *(#|$)' ./klipper-translations/active_translations)

# Build English
echo "building site for en"
mkdocs build -f "docs/_klipper3d/mkdocs.yml"

while IFS="," read dirname langsite langdesc langsearch; do
  echo "create language specific mkdocs configurations for ${langsite}"
  cp "${MKDOCS_DIR}mkdocs.yml" "${MKDOCS_DIR}${langsite}.yml"

  echo "replace search language"
  sed -i "s/.*#-# Search Language/      lang: $langsearch/" "${MKDOCS_DIR}${langsite}.yml"

  echo "replace site language"
  sed -i "s/.*#-# Site Language/  language: $langsite/" "${MKDOCS_DIR}${langsite}.yml"

  echo "set directories"
  sed -i "s/.*#-# Markdown File Directory/docs_dir: \'..\/$langsite\'/" "${MKDOCS_DIR}${langsite}.yml"
  sed -i "s/.*#-# Site Directory/site_dir: '..\\/..\\/site\\/$langsite'/" "${MKDOCS_DIR}${langsite}.yml"

  echo "building site for ${langsite}"
  mkdocs build -f "docs/_klipper3d/${langsite}.yml"
done <  <(egrep -v '^ *(#|$)' ./klipper-translations/active_translations)
