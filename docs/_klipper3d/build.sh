#!/bin/bash
# Modify the file structure before running mkdocs
# This is a make shift multi-site building script before
# the current structure of Klipper-translations can be
# directly utilized by MkDocs.

MKDOCS_DIR="docs/_klipper3d/"

#fetch translations
git clone --depth 1 https://github.com/Klipper3d/klipper-translations

#Move English Markdown Files
mkdir docs/en
mv docs/*.md docs/en/

while IFS="," read dirname langsite langdesc note; do
  new_local_dir="docs/${langsite}"
  local_dir="klipper-translations/docs/locales/$dirname"
  mkdir "$new_local_dir"
  # move and rename markdown files
  echo "Moving $dirname to $langsite"
  mv "$local_dir"/*.md "$new_local_dir"

  # manually replace index.md if a manual-index.md exist
  manual_index="${new_local_dir}/manual-index.md"
  if [[ -f "$manual_index" ]];then
    mv -f "$manual_index" "${new_local_dir}/index.md"
    echo "replaced index.md with manual_index.md for $langsite"
  else
      echo "Manually translated index file for $langsite not found!"
  fi

  # Insert entries to extra.yml for language switching
  echo  -e "    - name: ${langdesc}\n      link: /${langsite}/\n      lang: ${langsite}\n" >> "${MKDOCS_DIR}extra.yml"
done <  <(egrep -v '^ *(#|$)' ./klipper-translations/active_translations)

while IFS="," read dirname langsite langdesc langsearch; do
  echo "create language specific directory configurations"
  cat "${MKDOCS_DIR}base.yml" >> "${MKDOCS_DIR}${langsite}.yml"
  echo "replace search language"
  sed -i "s/en #\\*# Search Language/$langsearch/" "${MKDOCS_DIR}${langsite}.yml"
  echo "replace site language"
  sed -i "s/en #\\*# Site Language/$langsite/" "${MKDOCS_DIR}${langsite}.yml"
  echo "add directories"
  sed -i "s/'..\\/en' #\\*# Markdown File Directory/'..\\/$langsite'/" "${MKDOCS_DIR}${langsite}.yml"
  sed -i "s/'..\\/..\\/site\\/en' #\\*# Site Directory/'..\\/..\\/site\\/$langsite'/" "${MKDOCS_DIR}${langsite}.yml"

  # create language specific naviagtion table (TODO, reserved)
  cat "${MKDOCS_DIR}nav_en.yml" >> "${MKDOCS_DIR}${langsite}.yml"

  # copy extra.yml
  cat "${MKDOCS_DIR}extra.yml" >> "${MKDOCS_DIR}${langsite}.yml"

  # copy resources
  cp -r docs/img "docs/${langsite}/img"
  cp -r docs/prints "docs/${langsite}/prints"
  # build sites
  mkdocs build -f "docs/_klipper3d/${langsite}.yml"
done <  <(egrep -v '^ *(#|$)' ./klipper-translations/active_translations)

# Build English
cat "${MKDOCS_DIR}base.yml" > "${MKDOCS_DIR}en.yml"
cat "${MKDOCS_DIR}extra.yml" >> "${MKDOCS_DIR}en.yml"
cat "${MKDOCS_DIR}nav_en.yml" >> "${MKDOCS_DIR}en.yml"
mkdocs build -f "docs/_klipper3d/en.yml"

#Set en to default
mv -f site/en/* site/
