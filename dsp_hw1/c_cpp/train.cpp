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
	HMM hmm_initial;
	loadHMM( &hmm_initial, argv[2]);
	
	string iter = argv[1];
	int iteration = stoi(iter);

	for(int it = 0; it < iteration; it++){
		double first_gamma[6] = {0}, all_epsilon[6][6] = {0}, b_gamma[6] = {0}, a_gamma[6] = {0}, k_gamma[6][6] = {0};

		fstream str_file;
		string seq;
		str_file.open(argv[3]);
		int n_samples = 0;

		while(str_file >> seq){
			n_samples += 1;
			int seq_len = seq.length();
			double alpha[seq_len][hmm_initial.state_num] = {0};
			double beta[seq_len][hmm_initial.state_num] = {0};
			//Initialize
			for(int s = 0; s < hmm_initial.state_num; s++){
				alpha[0][s] = hmm_initial.initial[s] * hmm_initial.observation[int(seq[0]) - 65][s];
				beta[seq_len - 1][s] = 1;
			}
			//Induction
			for(int t = 1; t < seq_len; t++){
				for(int j = 0; j < hmm_initial.state_num; j++){
					double temp = 0;
					for(int i = 0; i < hmm_initial.state_num; i++){
						temp += (alpha[t-1][i] * hmm_initial.transition[i][j]);
						//beta[seq_len - 1 - t][j] += hmm_initial.transition[j][i] * hmm_initial.observation[int(seq[seq_len - t]) - 65][i] * beta[seq_len - t][i];
					}
					alpha[t][j] = temp * hmm_initial.observation[int(seq[t]) - 65][j];
				}
			}
			for(int t = seq_len - 2; t > -1; t--){
                for(int i = 0; i < hmm_initial.state_num; i++){
                	double temp = 0;
                    for(int j = 0; j < hmm_initial.state_num; j++){
                        temp += beta[t+1][j] * hmm_initial.transition[i][j] * hmm_initial.observation[int(seq[t+1])-65][j];
                    }
                    beta[t][i] = temp;
                }
            }
			//Calculate gamma, epsilon
			double gamma[seq_len][hmm_initial.state_num] = {0};
			double epsilon[seq_len][hmm_initial.state_num][hmm_initial.state_num] = {0};
			for(int t = 0; t < seq_len; t++){
				double denominator = 0;
				for(int j = 0; j < hmm_initial.state_num; j++){
					denominator += (alpha[t][j] * beta[t][j]);
				}
				for(int i = 0; i < hmm_initial.state_num; i++){
					gamma[t][i] = ((alpha[t][i] * beta[t][i]) / denominator);
				}
			}
			for(int t = 0; t < seq_len - 1; t++){
				double denominator = 0;
				for(int i = 0; i < hmm_initial.state_num; i++){
					for(int j = 0; j < hmm_initial.state_num; j++){
						denominator += (alpha[t][i] * hmm_initial.transition[i][j] * hmm_initial.observation[int(seq[t+1] - 65)][j] * beta[t+1][j]);
					}
				}
				for(int i = 0; i < hmm_initial.state_num; i++){
					for(int j = 0; j < hmm_initial.state_num; j++){
						epsilon[t][i][j] = (alpha[t][i] * hmm_initial.transition[i][j] * hmm_initial.observation[int(seq[t+1] - 65)][j] * beta[t+1][j] / denominator);
					}
				}
			}
			//Accumulate
			for(int i = 0; i < hmm_initial.state_num; i++){
				first_gamma[i] += gamma[0][i];
				for(int t = 0; t < seq_len - 1; t++){
					a_gamma[i] += gamma[t][i];
					b_gamma[i] += gamma[t][i];
					k_gamma[int(seq[t] - 65)][i] += gamma[t][i];
				}
				b_gamma[i] += gamma[seq_len - 1][i];
				k_gamma[int(seq[seq_len - 1] - 65)][i] += gamma[seq_len - 1][i];
			}
			for(int t = 0; t < seq_len - 1; t++){
				for(int i = 0 ; i < hmm_initial.state_num; i++){
					for(int j = 0; j < hmm_initial.state_num; j++){
						all_epsilon[i][j] += epsilon[t][i][j];
					}
				}
			}

		}
		str_file.close();
		//Update
		for(int i = 0; i < hmm_initial.state_num; i++){
			hmm_initial.initial[i] = (first_gamma[i] / n_samples);
			//cout << hmm_initial.initial[i] << endl;
			for(int j = 0; j < hmm_initial.state_num; j++){
				hmm_initial.transition[i][j] = (all_epsilon[i][j] / a_gamma[i]);
				//cout << hmm_initial.transition[i][j] << endl;
			}
			for(int k = 0; k < hmm_initial.observ_num; k++){
				hmm_initial.observation[k][i] = (k_gamma[k][i] / b_gamma[i]);
				//cout << hmm_initial.observation[k][i] << endl;
			}
		}
	}
	FILE *fout;
	fout = fopen(argv[4] , "w");
	dumpHMM( fout, &hmm_initial );
    fclose(fout);

    return 0; 
}