#include <stdio.h>
#include <stdlib.h>
#include "Ngram.h"
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>



static Vocab voc;
static int ngram_order = 2;
static Ngram lm( voc, ngram_order );

double getBigramProb(const char *w1, const char *w2)
{
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return lm.wordProb( wid2, context);
}
int main(int argc, char *argv[])
{
    File lmFile(argv[6], "r");
    lm.read(lmFile);
    lmFile.close();

    ifstream mapfile(argv[4]);
    map<string, string> table;
    string buf;
	while(getline(mapfile, buf)){
		string key = buf.substr(0, buf.find('\t'));
		string values = buf.substr(buf.find('\t') + 1, -1);
		table[key] = values;
	}
	mapfile.close();

	ifstream testfile(argv[2]);
	vector< vector<string> > testing_lines;
	while(getline(testfile, buf)){
		stringstream ss(buf);
		string dir;
		vector<string> temp_line;
		temp_line.push_back("<s>");
		while(getline(ss, dir, ' ')){
			if(dir.find_first_not_of(' ') != string::npos){
				temp_line.push_back(dir);
			}
		}
		temp_line.push_back("</s>");
		testing_lines.push_back(temp_line);
	}
	for(int l = 0; l < testing_lines.size(); l++){
		vector< vector<double> > path_prob;
		vector< vector<int> > path_idx;
		path_prob.push_back(vector<double>());
		path_idx.push_back(vector<int>());
		for(int w = 1; w < testing_lines[l].size(); w++){
			string cur_w = table[testing_lines[l][w]];
			string prev_w = table[testing_lines[l][w-1]];
			vector<string> cur_words;
			vector<string> prev_words;
			if(w != 1){
				stringstream sss(prev_w);
				string dirr;
				while(getline(sss, dirr, ' ')){
					if(dirr.find_first_not_of(' ') != string::npos){
						prev_words.push_back(dirr);
					}
				}
			}else{
				prev_words.push_back("<s>");
			}
			if(w != (testing_lines[l].size() - 1)){
				stringstream ss(cur_w);
				string dir;
				while(getline(ss, dir, ' ')){
					if(dir.find_first_not_of(' ') != string::npos){
						cur_words.push_back(dir);
					}
				}
			}else{
				cur_words.push_back("</s>");
			}
			vector<double> temp_path_prob;
			vector<int> temp_path_idx;
			for(int m = 0; m < cur_words.size(); m++){
				double max_prob = -2147483647;
				int max_prob_idx = 0;
				for(int n = 0; n < prev_words.size(); n++){
					double prob;
					if(w == 1){
						prob = getBigramProb(prev_words[n].c_str(), cur_words[m].c_str());
					}else{
						prob = path_prob[w - 1][n] + getBigramProb(prev_words[n].c_str(), cur_words[m].c_str());
					}
					if(prob > max_prob && prob < 0) {
                        max_prob = prob;
                        max_prob_idx = n;
                    }
				}
				temp_path_prob.push_back(max_prob);
				temp_path_idx.push_back(max_prob_idx);
			}
			path_prob.push_back(temp_path_prob);
			path_idx.push_back(temp_path_idx);
		}
		vector<string> backtrack_words;
		int max_prob_idx = 0;
		for(int w = testing_lines[l].size() - 1; w > 0; w--){
			string cur_w = table[testing_lines[l][w]];
			vector<string> cur_words;
			if(w != (testing_lines[l].size() - 1)){
				stringstream ss(cur_w);
				string dir;
				while(getline(ss, dir, ' ')){
					if(dir.find_first_not_of(' ') != string::npos){
						cur_words.push_back(dir);
					}
				}
			}else{
				cur_words.push_back("</s>");
			}
			backtrack_words.push_back(cur_words[max_prob_idx]);
			max_prob_idx = path_idx[w][max_prob_idx];
		}
		backtrack_words.push_back("<s>");
		for(int h = backtrack_words.size()-1 ; h >= 0; h--){
			cout << backtrack_words[h];
			if(h != 0){
				cout << " ";
			}
		}
		cout << endl;
	}    
    return 0;
}