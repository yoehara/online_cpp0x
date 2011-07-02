/*
 * Copyright (c) 2011 Yo Ehara
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
typedef int    fid_t;
typedef double val_t;
typedef int    clabel_t;
typedef vector<pair<fid_t, val_t>> fv_t;
typedef pair<clabel_t, fv_t>       datum_t;
typedef vector<datum_t>            data_t;

struct weight_t: vector<val_t>{
  weight_t(int size=0): vector<val_t>(size){}
  inline val_t operator*(const datum_t& datum) const{
    val_t sum = 0.0;
    for(const auto& p : datum.second)
        sum += (*this)[p.first]* p.second;
    return sum; 
  }
};

struct perceptron_t{
  val_t    margin;
  weight_t weight;
  perceptron_t(int size, val_t margin_=0.0): weight(size),  margin(margin_)   {}
  inline clabel_t classify(const datum_t& datum) const{ return weight*datum>0.0?1:-1; }
  inline void     train   (const datum_t& datum){
    if(((val_t)datum.first)*(weight*datum)<=margin)
      for(const auto& p : datum.second)
        weight[p.first] += ((val_t)datum.first)*p.second;
  }
};

fid_t readFile(data_t& data, const string& filename,  val_t bias=0.0, fid_t training_maxfid=-1){
  ifstream ifs(filename);
  fid_t maxfid=0;
  for(string line; getline(ifs, line);){
    istringstream ss(line);
    datum_t d; ss >> d.first;
    fid_t fid; char sep; val_t fval;
    while(ss >> fid >> sep >> fval){
      if(training_maxfid>=0 && fid>training_maxfid) continue;
      maxfid = max(maxfid, fid);
      d.second.push_back(move(make_pair(fid, fval)));
    }
    data.push_back(move(d));
  }
  ifs.close(); 
  fid_t fbias = (training_maxfid<0? (++maxfid) : training_maxfid);
  for(datum_t& datum : data) datum.second.push_back(make_pair(fbias, bias)); 
  return maxfid;
}

void printrate(const string& header, int a , int b){
  cout << header <<  a << '/' << b << '=' << ((float)a)/((float)b) << endl;
}

int main(int argc, char** argv){
  ostringstream oss;  while(oss << *++argv << ' '){}
  istringstream iss(oss.str()); 
  string trainingfile; iss >> trainingfile;
  string testfile;     iss >> testfile;
  int niteration=1;    iss >> niteration;
  val_t margin = 0.0;  iss >> margin;
  val_t bias   = 0.0;  iss >> bias; 

  data_t training_data; fid_t maxfid = readFile(training_data, trainingfile, bias)+1;   

  perceptron_t p(maxfid + 1, margin); srand(1);
  for(int i=0;i<niteration;++i) {
    random_shuffle(training_data.begin(), training_data.end()); 
    for(const datum_t& t : training_data) p.train(t);
  }

  data_t test_data; readFile(test_data, testfile, bias, maxfid);

  printrate("Accuracy: ",  count_if(test_data.begin(), test_data.end(), 
    [&p](const datum_t& t){return p.classify(t)==t.first;}),  test_data.size());
  printrate("Zerofication rate: ", count_if(p.weight.begin(), p.weight.end(), 
    [](const val_t& w_i ){return (bool)(w_i==0.0);}) , (int)maxfid+1);
} 

