#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <sstream>
#include <unordered_map>
#include "Ngram.h"
using namespace std;

int ngram_order = 2;
Vocab voc;
Ngram lm(voc, ngram_order);
unordered_map< string, vector<string> > dict;

class node{
    // constructor
    public:
        node():big5(" "), prob(-1e20), prev(NULL){}
        node(string s, double p = -1e20):big5(s), prob(p), prev(NULL){}
        string big5;
        int prev;
        double prob;
};

typedef vector<vector<node> > container;

double getBigramProp(const char* w1, const char* w2){
    // calculate P(W2|W1)
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);
    if (wid1 == Vocab_None) wid1 = voc.getIndex(Vocab_Unknown);
    if (wid2 == Vocab_None) wid2 = voc.getIndex(Vocab_Unknown);
    VocabIndex context[] = { wid1, Vocab_None };
    return lm.wordProb(wid2, context);
}

container preprocess(string test){
    container graph;
    stringstream ss(test);
    string inputStr;   
    vector<node> a;
    a.push_back(node("<s>", 0));
    graph.push_back(a);

    while(ss >> inputStr){
        vector<node> col;
        vector<string> choices = dict[inputStr];
        for(int i = 0; i < choices.size(); i++){
            node new_node = node(choices[i]);
            col.push_back(new_node);
        }
        graph.push_back(col);
        // cout << graph[0][0] -> big5 << endl;
        // getchar();
    }

    vector<node> b;
    b.clear();
    b.push_back(node("</s>"));
    graph.push_back(b);
    return graph;
}

void viterbi(container graph){
    // node start = node("<s>");
    
    for(int i = 1; i < graph.size(); i++){
        // at ith char
        // cout << "i = " << i << endl; 
        // cout << "size = " << graph[i].size() << endl;
        for(int j = 0; j < graph[i].size(); j++){
            node &curr_node = graph[i][j];

            // if(i == graph.size() - 1)
            //     cout << "curr_node.big5 " << curr_node.big5 << endl;
            
            for(int k = 0; k < graph[i - 1].size(); k++){
                node prev_node = graph[i - 1][k];
                // cout << prev_node -> big5 << endl;
                double sum_prob = prev_node.prob + getBigramProp(prev_node.big5.c_str(), curr_node.big5.c_str());
                // if(i == graph.size() - 1)
                // cout << "sum_prob " << sum_prob << endl;

                if(sum_prob > curr_node.prob){
                    curr_node.prob = sum_prob;
                    curr_node.prev = k;
                }
            }
        }
    }
    // debug
    // cout << "debug" << endl;
    // cout << "graph_size" << graph.size() << endl;
    // double max = -1e10;
    // vector<node> last_col = graph[graph.size() - 1];
    node last_node = graph[graph.size() - 1].back();
    // cout << graph[graph.size() - 1].size() << endl;
    // cout << last_node.prob << endl;
    // getchar();
    // for(int i = 0; i < last_col.size(); i++){
    //     // cout << i << endl;
    //     node curr_node = last_col[i];
    //     // cout << curr_node.prob << endl;
    //     // cout << curr_node.big5 << endl;
    //     if(curr_node.prob >= max){
    //         max = curr_node.prob;
    //         last_node = curr_node;
    //     }
    // }

    // cout << "hey" << endl;
    // cout << "last_node -> big5 " << last_node -> big5 << endl;

    node travel_node;
    travel_node = last_node;
    string ans = "";
    int i = graph.size() - 1;

    while(true){
        // cout << travel_node.big5 << endl;
        if(i == 0){
            ans = travel_node.big5 + ans;
        }else{
            ans = " " + travel_node.big5 + ans;
        }
        if(i > 0) travel_node = graph[--i][travel_node.prev];
        else break;
    }
    // ans = "<s> " + ans;
    cout << ans << endl;
    // getchar();
}

// disambig -text testdata/xx.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > $output
int main(int argc, char *argv[]) {
    // lm
    File lmFile(argv[6], "r");
    lm.read(lmFile);
    lmFile.close();

    // map
    ifstream mapFile(argv[4]);
    string big5, zhu, line;

    while(getline(mapFile, line)){
        stringstream ss(line);
        ss >> big5;
        while(ss >> zhu){
            dict[big5].push_back(zhu);
        }
    }
    
    // testfile
    ifstream testFile(argv[2]);
    while(getline(testFile, line)){
        container graph = preprocess(line);
        viterbi(graph);
    }
}