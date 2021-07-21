import re

def transform(markdown: str, page, config, files):
    in_list = False
    lines = markdown.splitlines()
    for i in range(len(lines)):
        lines[i] = lines[i].replace('](../',
                                    f"]({config['repo_url']}blob/master/")
        lines[i] = re.sub(r"\\", "<br>", lines[i])
        # check that lists at level 0 are not indented (no space before *|-|1.)
        if len(lines[i]) == 0:
            in_list = False
        elif re.match(r"^(\*|-|\d+\.) ", lines[i]):
            in_list = True
        if not in_list:
            lines[i] = re.sub(r"^\s+(\*|-|\d+\.) ", r"\1 ", lines[i])
    output = "\n".join(lines)
    return output
