#include <stdio.h>
#include <iostream>
#include <string>
#define LETTERS int(0x7f)
using namespace std;
using std::string;
/*
We will use an unordered_map (hashTable) with key and value,
 the key is char and the value is the char after munipilations of keygen.exe
*/
int main(int argc, char* argv[]){
    //string input = "IFRIZM9ZLR1CJWFH"; //check

    string key = "";
    char dictRev[LETTERS];
    string code = "5";
    code.push_back('`');
    code += ">AdIG-*8Ee}@BDRFkzw]ZYgrsm1q\\2|3Jj(KuX !':_v6aWf{y\"C;+L<S4HO[0?$bh";
    code.push_back('%');
    code += "/MtoT9,N)&l=xUpV^7~n#ciQP";
    string decode = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    //Create a dictionry of chars for each char 
    for(int i=0 ; i<(int)(0x7f)-(int)(0x21) ; i++){
        dictRev[code[i]]= (char)decode[i];
    }
    
    //Now lets traverse the argument - password user insert
    int j=0;
    while(argv[1][j] != '\0'){
        key.push_back(dictRev[argv[1][j++]]);
    } 
    /* Check
    while(input[j] != '\0'){
        key.push_back(dictRev[char(input[j++])]);
    } 
    */
    //print the result of the reversed keygen.exe
    std::cout << key ;
    return 0;
}