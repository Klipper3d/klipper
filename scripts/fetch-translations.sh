#!/bin/bash
# Modify the file structure before running mkdocs
# This is a make shift script before the current structure of
# Klipper-translations can be directly utilized by mkdocs
# Usage: pre-mkdocs.sh <lang_directory_name> <target_lang_name>

git clone --depth 1 https://github.com/Klipper3d/klipper-translations

cat ./klipper-translations/active_translations | while read dirname langname ; do
  local_dir=klipper-translations/docs/locales/$dirname
  echo "Moving $dirname to $langname"
  for file in $local_dir/*.md; do
    mdfilename=${file/$local_dir\//}
    mv $file "./docs/${mdfilename//.md/.${langname}.md}"
  done
done
