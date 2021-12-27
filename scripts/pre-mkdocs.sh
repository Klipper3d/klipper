#!/bin/bash
# Modify the file structure before running mkdocs
# This is a make shift script before the current structure of
# Klipper-translations can be directly utilized by mkdocs

git submodule add --force https://github.com/Klipper3d/klipper-translations

for dir in klipper-translations/docs/locales/*/; do

  dir=${dir%*/}
  lang=${dir##*/}
  if [ $lang = "zh-hans" ]; then
    echo "Moving $lang"
    for file in klipper-translations/docs/locales/$lang/*.md; do
      mdfilename=${file/$dir\//}
      mv $file "./docs/${mdfilename//.md/.zh.md}"
    done
  fi
done
