import gensim
from gensim.models import Word2Vec
from allennlp.modules.elmo import Elmo, batch_to_ids
from bert_serving.client import BertClient
import numpy as np
from elmoformanylangs import Embedder
import pickle



test_word = "數位語音處理概論"

def trans_w2v(text):
	model = Word2Vec.load("pretrained/zh.bin")
	vector = model.wv[text]
	print(vector)

def trans_elmo(text):
	options_file = "options.json" 
	weight_file = "weights.hdf5"
	elmo = Elmo(options_file, weight_file, 1, dropout=0)
	sentence_lists = [text]
	character_ids = batch_to_ids(sentences_lists)
	embeddings = elmo(character_ids)['elmo_representations']
	print(embeddings)

def trans_bert(dic):
	#bert-serving-start -model_dir chinese_L-12_H-768_A-12/
	sents = []
	for key in dic:
		sents.append(dic[key])
	bc = BertClient()
	vector = bc.encode(sents)
	print(vector.shape)
	i = 0
	for key in dic:
		dic[key] = vector[i]
		i += 1
	return dic

def trans_elmo(dic):
	e = Embedder('pretrained/')
	sents = []
	for key in dic:
		sents.append([dic[key]])
	vector = e.sents2elmo(sents, -1)
	print(len(vector))
	i = 0
	for key in dic:
		dic[key] = vector[i][0]
		i += 1
	return dic

def save_obj(obj, name):
    with open(name + '.pkl', 'wb') as f:
        pickle.dump(obj, f, pickle.HIGHEST_PROTOCOL)

def load_obj(name):
    with open(name + '.pkl', 'rb') as f:
        return pickle.load(f)


if __name__ == '__main__':
	course_dict = load_obj('course')
	course_dict = trans_elmo(course_dict)
	save_obj(course_dict, 'embedding/embedding_elmo')