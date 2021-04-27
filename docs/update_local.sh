
for file in *.md; do
  echo "Converting $file to ${file//.md/.po}"
  md2po $file -e utf-8 -w 71 -q -s --po-filepath local/zh-hans/${file//.md/.po}
  echo "$Converting ${file//.md/.po} to $file"
  po2md $file --md-encoding utf-8 --po-encoding utf-8 --pofiles local/zh-hans/${file//.md/.po} -q -s local/zh-hans/$file
done

