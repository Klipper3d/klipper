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

  # read toc
  title=$(sed -n '1p' ${locale_dir}/Navigation.md)
  installation_and_configuration=$(sed -n '3p' ${locale_dir}/Navigation.md)
  configuration_reference=$(sed -n '5p' ${locale_dir}/Navigation.md)
  bed_level=$(sed -n '7p' ${locale_dir}/Navigation.md)
  resonance_compensation=$(sed -n '9p' ${locale_dir}/Navigation.md)
  command_template=$(sed -n '11p' ${locale_dir}/Navigation.md)
  developer_documentation=$(sed -n '13p' ${locale_dir}/Navigation.md)
  device_specific_documents=$(sed -n '15p' ${locale_dir}/Navigation.md)

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

  echo "replace toc"
  sed -i "s%Klipper documentation$%${title}%" "${new_mkdocs_file}"
  sed -i "s%Installation and Configuration:$%${installation_and_configuration}:%" "${new_mkdocs_file}"
  sed -i "s%Configuration Reference:$%${configuration_reference}:%" "${new_mkdocs_file}"
  sed -i "s%Bed Level:$%${bed_level}:%" "${new_mkdocs_file}"
  sed -i "s%Resonance Compensation:$%${resonance_compensation}:%" "${new_mkdocs_file}"
  sed -i "s%Command templates:$%${command_template}:%" "${new_mkdocs_file}"
  sed -i "s%Developer Documentation:$%${developer_documentation}:%" "${new_mkdocs_file}"
  sed -i "s%Device Specific Documents:$%${device_specific_documents}:%" "${new_mkdocs_file}"

  # Build site
  echo "building site for ${langsite}"
  mkdir -p "${PWD}/site/${langsite}/"
  ln -sf "${PWD}/site/${langsite}/" "${WORK_DIR}lang/${langsite}/site"
  mkdocs build -f "${new_mkdocs_file}"
done < <(egrep -v '^ *(#|$)' ${TRANS_FILE})
