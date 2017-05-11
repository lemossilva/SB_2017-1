// reading a text file
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main () {
  	string line, rotulo;
  	ifstream myfile ("retangulo.asm");
  	ofstream tab_simb ("Ts.txt");
  	int pos,i;


  	if (!tab_simb.is_open() || ! myfile.is_open()){
  		cout<<"arquivo não abriu"<< endl;
  		return 0;
  	}

  	while ( getline (myfile,line) ){
      	cout << line << '\n';
      	pos = line.find(":");
      	if(pos >0){
      		for(i=0;i<pos;i++){
      			rotulo = line.at(i);
      			cout<<rotulo;
      		}
      			cout<<endl;



  	  	}


    }

	myfile.close();
    tab_simb.close();

  	return 0;
}
