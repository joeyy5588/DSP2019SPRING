#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hmm.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	HMM hmms[5];
	load_models( argv[1], hmms, 5);

	fstream str_file;
	string seq;
	str_file.open(argv[2]);
	fstream out_file;
	out_file.open(argv[3], ios::out);
	int n_samples = 0;

	while(str_file >> seq){
		int seq_len = seq.length();
		int max_index = -1;
		double max_p = 0;
		
		for(int m = 0; m < 5; m++){
			double delta[seq_len][6] = {0.0};
			int s_num = hmms[m].state_num;
			//Initialize
			for(int i = 0; i < s_num; i++){
				delta[0][i] = hmms[m].initial[i] * hmms[m].observation[int(seq[0]) - 65][i];
			}
			//Recursion
			for(int t = 1; t < seq_len; t++){
				for(int j = 0; j < s_num; j++){
					double max_temp = 0.0;
					for(int i = 0; i < s_num; i++){
						double temp = delta[t-1][i] * hmms[m].transition[i][j];
						if(max_temp < temp) max_temp = temp;
					}
					delta[t][j] = max_temp * hmms[m].observation[int(seq[t]) - 65][j];
				}
			}
			double max_p_temp = 0.0;
			for(int i = 0; i < s_num; i++){
				if(max_p_temp < delta[seq_len - 1][i])max_p_temp = delta[seq_len - 1][i];
			}
			if(max_p < max_p_temp){
				max_p = max_p_temp;
				max_index = m;
			}
		}
		out_file << hmms[max_index].model_name << " " << max_p << endl;
		
	}

	str_file.close();
	out_file.close();

	return 0;
}