#!/bin/bash
# Modify the file structure before running mkdocs
# This is a make shift script before the current structure of 
# Klipper-translations can be directly utilized by mkdocs

git submodule add --force https://github.com/Klipper3d/klipper-translations

for dir in klipper-translations/docs/locales/*/; do
  
  dir=${dir%*/}
  lang=${dir##*/}
  
  echo "Moving $lang"
  for file in klipper-translations/docs/locales/$lang/*.md; do
  
    mdfilename=${file/$dir\//}

    # This is a make shift matching, in actual use only match the languages
    # we want to include.
    if [ $dir != "en" ]; then 
      mv $file "./docs/${mdfilename//.md/.$lang.md}"  
    fi
  done
done