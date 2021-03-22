#include<textquery.h>
#include<fstream>
#include<string>

using namespace std;

int main(){
    string filename = "test.txt";
    ifstream inputfile;
    inputfile.open(filename);
    runQueries(inputfile);
    inputfile.close();
    return 0;
}
