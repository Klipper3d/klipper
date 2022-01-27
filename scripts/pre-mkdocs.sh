#!/bin/bash
# Modify the file structure before running mkdocs
# This is a make shift script before the current structure of
# Klipper-translations can be directly utilized by mkdocs
# Usage: pre-mkdocs.sh <lang_directory_name> <target_lang_name>

git submodule add --force https://github.com/Klipper3d/klipper-translations
dir=klipper-translations/docs/locales/$1
echo "Moving $1"
for file in $dir/*.md; do
  mdfilename=${file/$dir\//}
  mv $file "./docs/${mdfilename//.md/.$2.md}"
done
