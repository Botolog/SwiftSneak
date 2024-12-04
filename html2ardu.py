import os 

PREFIX = "client.println(\""
SUBFIX = "\");\n"
def convert_line(line):
  newline = ""
  for char in line:
    if char == "\"": newline += "\\"
    newline += char
  return PREFIX + newline[:-1] + SUBFIX

if __name__ == "__main__":
  html_file = open("html", "r", encoding="utf-8")
  cpp_file = open("cpp", "w", encoding="utf-8")

  html_lines = html_file.readlines()
  for line in html_lines:
    newline = convert_line(line)
    cpp_file.write(newline)

  html_file.close()
  cpp_file.close()
