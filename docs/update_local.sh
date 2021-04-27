#!/bin/bash
#Automatically generate a set of po and md files for Localization
#in the ./local/[language]/ folder
#requirement: pip install mdpo

for dir in ./local/*/
do
    dir=${dir%*/}
    echo "Updating ${dir##*/}"
    for file in *.md; do
      echo "Converting $file to ${file//.md/.po}"
      md2po $file --md-encoding utf-8 --po-encoding utf-8 \
      -e utf-8 -w 71 -q -s --po-filepath local/${dir##*/}/${file//.md/.po}
      echo "Converting ${file//.md/.po} to $file"
      po2md $file --md-encoding utf-8 --po-encoding utf-8 \
      --pofiles local/${dir##*/}/${file//.md/.po} -q -s local/${dir##*/}/$file
    done
done
