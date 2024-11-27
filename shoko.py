OUTPUTPATH = "./output/"

PREFIX = "client.println(\""
SUBFIX = "\");\n"


def conv(INPUT):
  html_file = open(INPUT, "r")
  cpp_file = open(OUTPUTPATH + INPUT, "w")

  html_lines = html_file.readlines()
  for line in html_lines:
    newline = ""
    for char in line:
      if char == "\"": newline += "\\"
      newline += char
    cpp_file.write(PREFIX + newline[:-1] + SUBFIX)
  return INPUT
