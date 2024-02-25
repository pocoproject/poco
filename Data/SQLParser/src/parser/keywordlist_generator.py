from __future__ import print_function
import math


with open("sql_keywords.txt", 'r') as fh:
	keywords = [line.strip() for line in fh.readlines() if not line.strip().startswith("//") and len(line.strip()) > 0]

	keywords = sorted(set(keywords)) # Sort by name
	keywords = sorted(keywords, key=lambda x: len(x), reverse=True) # Sort by length

	#################
	# Flex

	max_len = len(max(keywords, key=lambda x: len(x))) + 1
	max_len = 4 * int(math.ceil(max_len / 4.0))

	for keyword in keywords:
		len_diff = (max_len) - len(keyword)
		num_tabs = int(math.floor(len_diff / 4.0))

		if len_diff % 4 != 0: num_tabs += 1

		tabs = ''.join(['\t' for _ in range(num_tabs)])
		print("%s%sTOKEN(%s)" % (keyword, tabs, keyword))

	#
	#################


	#################
	# Bison
	line = "%token"
	max_len = 60

	print("/* SQL Keywords */")
	for keyword in keywords:

		if len(line + " " + keyword) > max_len:
			print(line)
			line = "%token " + keyword
		else:
			line = line + " " + keyword
	print(line)

	#
	#################
