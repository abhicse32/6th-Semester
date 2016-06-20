import re
read_from=open("data.txt","r")
write_to="combined2.txt"
with open(write_to,"w") as fout_:
	for lines in read_from:
		if lines[0]!='@':
			match=re.search(r"[a-zA-Z][a-zA-Z. ]*[a-zA-Z.]",lines)
			#print match.group()
			fout_.write(match.group())
			fout_.write('\n')
		else:
			fout_.write(lines)
