# DSP HW1

## 電機三 B05901082 楊晟甫

### 1. Environment

Win10 + Cygwin

### 2. Execution
```shell
#For train
./train $iter model_init.txt seq_model_01.txt model_01.txt
#For test
./test modellist.txt testing_data1.txt result1.txt
```
### 3. Summary

這次的作業還蠻有趣的，在聽完HMM很迷糊之後直接實作可以直截了當地了解到這個model在幹嘛，以及各種機率的計算那些式子意義為何。程式碼的部分就是照著投影片以及上課講義的公式實作，基本上沒有太大的問題。

整體來說在實作上經過**1000**次iteration後得到了和投影片一樣的結果(**Acc: 0.8696**)，是相當不錯的準確率，但考量到這個task的難易程度，也許還有往上成長的空間，也許針對initial model的調變可以增加準確率，或是畫出類似confusion matrix的圖來檢視是哪些model在判斷上容易出錯，進而讓結果更好，或是最簡單的，加高training iteration的次數。

下方是計算alpha, beta, gamma, epsilon的程式碼

```c++
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
```