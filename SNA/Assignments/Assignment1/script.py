file_=open("data.txt","r")
fname="combined2.txt"
with open(fname,'a') as fout_:
	for line in file_:
		for word in line.split():
			try:
				if int(word)/10:
					break
			except:
				if word[-1:]==':':
					fout_.write(word[:-1])
					fout_.write("\n");
					break
				elif word[-1:]!=']' or word[:1]!='[':
					fout_.write(word)