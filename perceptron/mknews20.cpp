#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
int main(){
  vector<string> rawdata;
  for(string line; getline(cin, line) ;){
    rawdata.push_back(line);
  }
  srand(1);
  random_shuffle(rawdata.begin(), rawdata.end());
  ofstream ofs_train("news20.train");
  int ntrain = 15000; int ntest = 4996;
  for(int i=0;i<ntrain;++i) ofs_train << rawdata[i] << endl;
  ofs_train.close();
  ofstream ofs_test("news20.test");
  for(int i=ntrain;i<ntrain+ntest; ++i) ofs_test << rawdata[i] << endl;
  ofs_test.close();
}
