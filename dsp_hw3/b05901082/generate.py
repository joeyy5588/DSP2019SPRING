# coding=Big5
import sys
import codecs

table = {
	u'\u3105': [], u'\u3106': [], u'\u3107': [], u'\u3108': [], u'\u3109': [], u'\u310a': [], u'\u310b': [], u'\u310c': [], u'\u310d': [], \
	u'\u310e': [], u'\u310f': [], u'\u3110': [], u'\u3111': [], u'\u3112': [], u'\u3113': [], u'\u3114': [], u'\u3115': [], u'\u3116': [], \
	u'\u3117': [], u'\u3118': [], u'\u3119': [], u'\u3127': [], u'\u3128': [], u'\u3129': [], u'\u311a': [], u'\u311b': [], u'\u311c': [], \
	u'\u311e': [], u'\u3120': [], u'\u3121': [], u'\u3122': [], u'\u3123': [], u'\u3124': [], u'\u3125': [], u'\u3126': []
}

with codecs.open(sys.argv[1], 'r', 'big5-hkscs') as file:
	for line in file:
		c_char = line.split(' ')[0]
		phonetic = line.split(' ')[1].split('/')
		head = list(set([x[0] for x in phonetic]))
		for h in head:
			table[h].append(c_char)
		table[c_char] = c_char

with codecs.open(sys.argv[2], 'w', 'big5-hkscs', errors='ignore') as outfile:
	for key, value in table.items():
		line = key + '\t'
		outfile.write(line)
		for idx, x in enumerate(value):
			if idx == (len(value) - 1):
				outfile.write(x + '\n')
			else:
				outfile.write(x + ' ')