#!/bin/bash
# Modify the file structure before running mkdocs
# This is a make shift script before the current structure of
# Klipper-translations can be directly utilized by mkdocs

git clone --depth 1 https://github.com/Klipper3d/klipper-translations

while IFS="," read dirname langname langdesc note; do
  # move and rename markdown files
  local_dir="klipper-translations/docs/locales/$dirname"
  echo "Moving $dirname to $langname"
  for file in "$local_dir"/*.md; do
    mdfilename="${file/$local_dir\//}"
    mv "$file" "./docs/${mdfilename//.md/.${langname}.md}"
  done

  # manually replace index.md if a manual-index.md exist
  manual_index="./docs/manual-index.$langname.md"

  if [[ -f "$manual_index" ]];then
    mv "$manual_index" "./docs/index.${langname}.md"
    echo "replaced index.${langname}.md with $manual_index"
  else
      echo "Manually translated index file for $dirname not found!"
  fi

  # add to translations.yml
  echo "      ${langname}: ${langdesc}" >> ./docs/_klipper3d/translations.yml
done <  <(egrep -v '^ *(#|$)' ./klipper-translations/active_translations)
