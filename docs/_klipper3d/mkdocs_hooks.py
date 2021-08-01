# Tool to customize conversion of markdown files during mkdocs site generation
import re
import logging

logger = logging.getLogger('mkdocs.mkdocs_hooks.transform')

def transform(markdown: str, page, config, files):
    in_code_block = 0
    in_list = False
    lines = markdown.splitlines()
    for i in range(len(lines)):
        line_out = lines[i]
        in_code_block = (in_code_block +
            len(re.findall("\s*[`]{3,}", line_out))) % 2
        if not in_code_block:
            line_out = line_out.replace('](../',
                                        f"]({config['repo_url']}blob/master/")
            line_out = re.sub("\\\s*$", "<br>", line_out)
            # check that lists at level 0 are not indented
            # (no space before *|-|1.)
            if len(line_out) == 0:
                in_list = False
            elif re.match(r"^(\*|-|\d+\.) ", line_out):
                in_list = True
            if not in_list:
                line_out = re.sub(r"^\s+(\*|-|\d+\.) ", r"\1 ", line_out)
            if line_out != lines[i]:
                logger.debug((f'[mkdocs_hooks] rewrite line {i+1}: '
                        f'"{lines[i]}" -> "{line_out}"'))
            lines[i] = line_out
    output = "\n".join(lines)
    return output
