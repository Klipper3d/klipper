
for file in *.md; do
  echo "Converting $file to ${file//.md/.po}"
  md2po $file -e utf8 -w 71 -q -s --po-filepath local/zh-hans/${file//.md/.po}
  echo "$Converting ${file//.md/.po} to $file"
  po2md $file --pofiles local/zh-hans/${file//.md/.po} -q -s local/zh-hans/README.md
done

