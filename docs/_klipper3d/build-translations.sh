#!/bin/bash
# This script extracts the Klipper translations and builds multiple
# mdocs sites - one for each supported language.  See the README file
# for additional details.

MKDOCS_DIR="docs/_klipper3d/"
WORK_DIR="work/"
TRANS_DIR="${WORK_DIR}klipper-translations/"
TRANS_FILE="${TRANS_DIR}active_translations"
MKDOCS_MAIN="${MKDOCS_DIR}mkdocs-main.yml"

# Fetch translations
git clone --depth 1 https://github.com/Klipper3d/klipper-translations ${TRANS_DIR}

# Create new mkdocs-main.yml with language links
cp ${MKDOCS_DIR}mkdocs.yml ${MKDOCS_MAIN}
while IFS="," read dirname langsite langdesc langsearch; do
  sed -i "s%^.*# Alternate language links automatically added here$%    - name: ${langdesc}\n      link: /${langsite}/\n      lang: ${langsite}\n\0%" ${MKDOCS_MAIN}
done < <(egrep -v '^ *(#|$)' ${TRANS_FILE})

# Build main English website
echo "building site for en"
mkdocs build -f ${MKDOCS_MAIN}

# Build each additional language website
while IFS="," read dirname langsite langdesc langsearch; do
  new_docs_dir="${WORK_DIR}lang/${langsite}/docs/"
  locale_dir="${TRANS_DIR}/docs/locales/${dirname}"

  # Copy markdown files to new_docs_dir
  echo "Copying $dirname to $langsite"
  mkdir -p "${new_docs_dir}"
  cp "${locale_dir}"/*.md "${new_docs_dir}"
  echo "copy resources"
  cp -r docs/img "${new_docs_dir}"
  cp -r docs/prints "${new_docs_dir}"
  cp -r docs/_klipper3d "${new_docs_dir}"

  # manually replace index.md if a manual-index.md exist
  manual_index="${new_docs_dir}manual-index.md"
  if [[ -f "${manual_index}" ]]; then
    mv -f "${manual_index}" "${new_docs_dir}index.md"
    echo "replaced index.md with manual_index.md for $langsite"
  else
    echo "Manually translated index file for $langsite not found!"
  fi

  # Create language specific mkdocs-lang-xxx.yml file
  echo "create language specific mkdocs configurations for ${langsite}"
  new_mkdocs_file="${new_docs_dir}_klipper3d/mkdocs-lang-${langsite}.yml"
  cp "${MKDOCS_MAIN}" "${new_mkdocs_file}"

  echo "replace search language"
  sed -i "s%^    lang: en$%    lang: ${langsearch}%" "${new_mkdocs_file}"

  echo "replace site language"
  sed -i "s%^  language: en$%  language: ${langsite}%" "${new_mkdocs_file}"

  # Build site
  echo "building site for ${langsite}"
  mkdir -p "${PWD}/site/${langsite}/"
  ln -sf "${PWD}/site/${langsite}/" "${WORK_DIR}lang/${langsite}/site"
  mkdocs build -f "${new_mkdocs_file}"
done < <(egrep -v '^ *(#|$)' ${TRANS_FILE})
