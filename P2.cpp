#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <cstring>


using namespace std;

void display(stack<char> z);

bool g_program = false;
bool g_begin = false;
bool g_write = false;
bool g_end = false;
bool g_var = false;
bool g_integer = false;
bool g_comma = false;
bool g_assign = false;
bool g_semi = false;

string CommentRemoval(string s){
    string removedComments = "";
    bool blockComment = false;

    for(int i = 0; i < s.length(); ++i){
        if(s[i] == '/' && s[i+1] == '*'){
            blockComment = true;
            i += 2;
        }
        else if(blockComment == true){
            if(s[i] == '*' && s[i+1] == '/'){
                blockComment = false;
                i += 2;
            }
        }
        if(!(blockComment))
            removedComments += s[i];
    }
    return removedComments;
}

string SpaceRemoval(string s){
    string removedSpaces = "";
    
    for(int i = 0; i < s.length(); ++i){
        if(s[i] == ' '){
            while(s[i+1] == ' '){
                ++i;
            }
        }
        if(s[i] == '\n'){
            if(s[i+1] == '\n'){
                ++i;
            }
        }
        removedSpaces += s[i];
    }
    return removedSpaces;
}

string Spaces(string s){
    bool space = false;
    bool space2 = false;
    int semi_count = 0;
    int assign_count = 0;
    int comma_count = 0;
    string SpacesAdded = "";
    for(int i = 0; i < s.length(); ++i){
        if(s[i] != ' ')
            if(s[i+1]==';' || s[i+1]==',' || (s[i+1]=='=' && s[i]!='e') || s[i+1]=='*' || s[i+1] == ':' || s[i+1]=='+' || s[i+1]==')'){// || s[i+1]=='(')
                space = true;
                if(s[i+1] == ';')
                    ++semi_count;
                if(semi_count == 8)
                    g_semi = true;
            }
        if(s[i] == ',' || (s[i] == '=' && s[i+1] != '"') || s[i] == '*' || s[i] == ':' || s[i] == '+' || s[i] == '('){// || s[i] == ')')
            space2 = true;
            if(s[i] == ','){
                ++comma_count;
                if(comma_count == 4)
                    g_comma = true;
            }
            if(s[i] == '='){
                ++assign_count;
            }
        if(assign_count == 4)
            g_assign = true;
        }
        
        SpacesAdded += s[i]; 
        if(space == true){
            SpacesAdded += ' ';
            space = false;
        }
        if(space2 == true){
            SpacesAdded += ' ';
            space2 = false;
        }
    } 
    return SpacesAdded;
}

string ReservedWSpaces(string s){
    bool program = false;
    bool write = false;
    bool integer = false;
    bool end = false;
    int writeCount = 0;
    string SpacesAdded2 = "";
    
    for(int i = 0; i < s.length(); ++i){
        if(s[i]=='p' && s[i+1]=='r' && s[i+2]=='o' && s[i+3]=='g' && s[i+4]=='r' && s[i+5]=='a' && s[i+6]=='m')
            program = true;
        if(s[i]=='i' && s[i+1]=='n' && s[i+2]=='t' && s[i+3]=='e' && s[i+4]=='g' && s[i+5]=='e' && s[i+6]=='r')
            integer = true;
        if(s[i]=='w' && s[i+1]=='r' && s[i+2]=='i' && s[i+3]=='t' && s[i+4]=='e'){
            write = true;
            ++writeCount;
        }
        if(s[i]=='e' && s[i+1]=='n' && s[i+2]=='d' && s[i+3]=='.')
            end = true;
        if(s[i]=='v' && s[i+1]=='a' && s[i+2]=='r' && s[i+3]=='\n')
            g_var = true;
        if(s[i]=='b' && s[i+1]=='e' && s[i+2]=='g' && s[i+3]=='i' && s[i+4]=='n' && s[i+5]=='\n')
            g_begin = true;

        SpacesAdded2 += s[i];

        if(program == true && s[i] == 'm' && s[i+1] != ' '){
            SpacesAdded2 += ' ';
            g_program = true;
            program = false;
        }
        if(write == true && s[i] == 'e' && s[i+1] != ' '){
            SpacesAdded2 += ' ';
            write = false;
            if(writeCount == 2)
                g_write = true;
        }
        if(end == true && s[i] == '.' && s[i+1] != ' '){ // might reach end of file here
            SpacesAdded2 += ' ';
            g_end = true;
            end = false;
        }
        if(integer == true && s[i] == 'r')
            g_integer = true;

    }
    return SpacesAdded2;
}

void MispelledOrMissing(){
    if (g_begin == false)
        cout << "begin missing or mispelled\n";
    if (g_end == false)
        cout << "end. missing or mispelled\n";
    if (g_integer == false)
        cout << "integer missing or mispelled\n";
    if (g_program == false)
        cout << "program missing or mispelled\n";
    if (g_var == false)
        cout << "var missing or mispelled\n";
    if (g_write == false)
        cout << "write missing or mispelled\n";
    if (g_assign == false)
        cout << "unknown identifier\n";
    if (g_comma == false)
        cout << ", missing\n";
    if( g_semi == false)
        cout << "; missing\n";
    if(g_assign == true && g_begin == true && g_comma == true && g_end == true && g_integer == true && g_program == true
        && g_semi == true && g_var == true && g_write == true)
          cout << "No errors\n";
}

int main(){

    string sFile = "";
    string rmSFile = "";
    string rmSp = "";
    vector<string> fileStrings;

    fstream txtFile;
    fstream newTxtFile2;

    ofstream newTxtFile("finalp2_2.txt");
    txtFile.open("finalp1.txt", ios::in);

    stringstream sBuff;
    sBuff << txtFile.rdbuf();
    sFile = sBuff.str();
    sFile = CommentRemoval(sFile);
    sFile = SpaceRemoval(sFile);
    sFile.erase(remove(sFile.begin(), sFile.end(), ' '), sFile.end());
    sFile = SpaceRemoval(sFile);
    sFile = Spaces(sFile);
    sFile = ReservedWSpaces(sFile);
    MispelledOrMissing();
    cout << sFile;
    newTxtFile << sFile;
    newTxtFile.close();

    string sFile2 = "";
    fstream txtFile3;
    txtFile3.open("finalp2_2.txt");
    stringstream sBuff2;
    sBuff2 << txtFile3.rdbuf();
    sFile2 = sBuff2.str();

    char test[166];
    strcpy(test, sFile2.c_str());
   // test[166] = '$';
   // for(int i = 0; i < 167; ++i){
   //     cout << test[i];
   // }

    int testpoint = 0;
    bool loop = 1;

    //This holds the current terminal and current popped nonterminal
    char x = ' ', y = ' ';
    int intx, inty, intloc;

    //array to hold table (did it in integers) + plus row and column size
    int array [19][27] = {
                        {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //P
                        {11, 11, 11, 11, 11, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //I
                        {111, 111, 111, 111, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //X
                        {2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //D
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //M
                        {22, 22, 22, 22, 22, 22, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Y
                        {222, 222, 222, 222, 222, 222, 444, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //S
                        {0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //W
                        {3, 3, 3, 3, 3, 3, 0, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Q
                        {33, 33, 33, 33, 33, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //A
                        {333, 333, 333, 333, 333, 333, 0, 0, 333, 333, 0, 0, 333, 0, 0, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333}, //E
                        {0, 0, 0, 0, 0, 0, 0, 0, 555, 6, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0}, //E'
                        {4, 4, 4, 4, 4, 4, 0, 0, 4, 4, 0, 0, 4, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, //T
                        {0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 66, 666, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3}, //T'
                        {44, 44, 44, 44, 44, 44, 0, 0, 7, 7, 0, 0, 77, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //F
                        {0, 0, 0, 0, 0, 0, 0, 0, 777, 777, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //#
                        {0, 0, 0, 0, 0, 0, 0, 0, 8, 88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //G
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 888, 9, 90, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999, 0, 0}, //P
                        {'a', 'b', 'c', 'd', 'w', 'f', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //L
                        };
// 1 = program I; var X begin Y end.
//11 = LL | LP
//111= D : M
// 2 = I,D | I
//22 = S | S Y
//222= A
// 3 = lam
//33 = I = E;
//333= TE'
// 4 = FT'
//44 = I
//444= W
//a = a
//b = b
//c = c
//d = d
//w = w
//f = f
// 5 = w(Q I);
//55 = "value=",
//555= +TE'
// 6 = -TE'
//66 = *FT'
//666= /FT'
// 7 = #
//77 = (E)
//777= GPP
// 8 = +
//88 = -
// 888 = 0
// 9 = 1
// 99 = 2
// 999 = 3
// 9999 = 4
// 99999 = 5
// 999999 = 6
// 9999999 = 7
// 99999999 = 8
// 999999999 = 9
    //starting the stack
    stack<char> s;
    s.push('$');
    s.push('P');
    display(s);

    //the loop that tests to see if the function given works in the language
    while (y != '$') {

      y = test[testpoint++];

      do {

        x = s.top();
        std::cout << x << y << '\n';
        s.pop();

        switch (x) {
          case 'P':
            cout << "case 'P'\n";
            intx = 0;
            break;
          case 'I':
            cout << "case 'I'\n";
            intx = 1;
            break;
          case 'X':
            cout << "case 'X'\n";
            intx = 2;
            break;
          case 'D':
            cout << "case 'D'\n";
            intx = 3;
            break;
          case 'M':
            cout << "case 'M'\n";
            intx = 4;
            break;
          case 'Y':
            cout << "case 'Y'\n";
            intx = 5;
            break;
          case 'S':
            cout << "case 'S'\n";
            intx = 6;
            break;
          case 'W':
          cout << "case 'W'\n";
            intx = 7;
            break;
          case 'Q':
            cout << "case 'Q'\n";
            intx = 8;
            break;
          case 'A':
            cout << "case 'A'\n";
            intx = 9;
            break;
          case 'E':
            cout << "case 'E'\n";
            intx = 10;
            break;
          case 'K':
            cout << "case 'K'\n";
            intx = 11;
            break;
          case 'T':
            cout << "case 'T'\n";
            intx = 12;
            break;
          case 'R':
            cout << "case 'R'\n";
            intx = 13;
            break;
          case 'F':
            cout << "case 'F'\n";
            intx = 14;
            break;
          case '#':
            cout << "case '#'\n";
            intx = 15;
            break;
          case 'G':
            cout << "case 'G'\n";
            intx = 16;
            break;
          case 'B':
            cout << "case 'B'\n";
            intx = 17;
            break;
          case 'L':
            cout << "case 'L'\n";
            intx = 18;
            break;
        }
        switch (y) {
          case 'a':
            cout << "case 'a'\n";
            inty = 0;
            break;
          case 'b':
            cout << "case 'b'\n";
            inty = 1;
            break;
          case 'c':
            cout << "case 'c'\n";
            inty = 2;
            break;
          case 'd':
            cout << "case 'd'\n";
            inty = 3;
            break;
          case 'w':
            cout << "case 'w'\n";
            inty = 4;
            break;
          case 'f':
            cout << "case 'f'\n";
            inty = 5;
            break;
          case 'g':
            cout << "case 'g'\n";
            inty = 6;
            break;
          case 'v':
            cout << "case 'v'\n";
            inty = 7;
            break;
          case '+':
            cout << "case '+'\n";
            inty = 8;
            break;
          case '-':
            cout << "case '-'\n";
            inty = 9;
            break;
          case '*':
            cout << "case '*'\n";
            inty = 10;
            break;
          case '/':
            cout << "case '/'\n";
            inty = 11;
            break;
          case '(':
            cout << "case '('\n";
            inty = 12;
            break;
          case ')':
            cout << "case ')'\n";
            inty = 13;
            break;
          case '$':
            cout << "case '$'\n";
            inty = 14;
            break;
          case '0':
            cout << "case '0'\n";
            inty = 15;
            break;
          case '1':
            cout << "case '1'\n";
            inty = 16;
            break;
          case '2':
            cout << "case '2'\n";
            inty = 17;
            break;
          case '3':
            cout << "case '3'\n";
            inty = 18;
            break;
          case '4':
            cout << "case '4'\n";
            inty = 19;
            break;
          case '5':
            cout << "case '5'\n";
            inty = 20;
            break;
          case '6':
            cout << "case '6'\n";
            inty = 21;
            break;
          case '7':
            cout << "case '7'\n";
            inty = 22;
            break;
          case '8':
            cout << "case '8'\n";
            inty = 23;
            break;
          case '9':
            cout << "case '9'\n";
            inty = 24;
            break;
          case ';':
            cout << "case ';'\n";
            inty = 25;
            break;
          case ':':
            cout << "case ':'\n";
            inty = 26;
            break;
        }

        intloc = array[intx][inty];

        switch (intloc) {
          case 0:
            std::cout << "This does not work in the given language" << '\n';
          //  return 0;
          case 1:
            cout << "case 1\n";
            s.push('p');
            s.push('I');
            s.push(';');
            s.push('v');
            s.push('X');
            s.push('b');
            s.push('Y');
            s.push('e');
            s.push('.');
            break;
          case 11:
            cout << "case 11\n";
            s.push(')');
            s.push('E');
            s.push('(');
            break;
          case 111:
            cout << "case 111\n";
            s.push('D');
            s.push(':');
            s.push('M');
            break;
          case 2:
            cout << "case 2\n";
            s.push('Q');
            s.push('T');
            break;
          case 22:
            cout << "case 22\n";
            s.push('Q');
            s.push('T');
            s.push('+');
            break;
          case 222:
            cout << "case 222\n";
            s.push('A');
            break;
          case 3:
            cout << "case 3\n";
            s.push('l');
            break;
          case 33:
            cout << "case 333\n";
            s.push('I');
            s.push('=');
            s.push('E');
            s.push(';');
            break;
          case 333:
            cout << "case 333\n";
            s.push('T');
            s.push('E');
            s.push('^');
            break;
          case 4:
            cout << "case 4";
            s.push('F');
            s.push('T');
            s.push('^');
            break;
          case 44:
            cout << "case 44\n";
            s.push('I');
            break;
          case 'a':
            cout << "case 'a'\n";
            s.push('a');
            break;
          case 'b':
            cout << "case 'b'\n";
            s.push('b');
            break;
          case 'c':
            cout << "case 'c'\n";
            s.push('c');
            break;
          case 'd':
            cout << "case 'd'\n";
            s.push('d');
            break;
          case 'w':
            cout << "case 'w'\n";
            s.push('w');
            break;
          case 'f':
            cout << "case 'f'\n";
            s.push('f');
            break;
          case 5:
            cout << "case 'w'\n";
            s.push('w');
            s.push('(');
            s.push('Q');
            s.push('I');
            s.push(')');
            break;
          case 55:
            cout << "case 55\n";
            s.push('"');
            s.push('v');
            s.push('=');
            s.push('"');
            s.push(',');
            break;
          case 555:
            cout << "case 555\n";
            s.push('+');
            s.push('T');
            s.push('E');
            s.push('^');
            break;
          case 6:
            cout << "case 6\n";
            s.push('-');
            s.push('T');
            s.push('E');
            s.push('^');
            break;
          case 66:
            cout << "case 66\n";
            s.push('*');
            s.push('F');
            s.push('T');
            s.push('^');
            break;
          case 666:
            cout << "case 666\n";
            s.push('/');
            s.push('F');
            s.push('T');
            s.push('^');
            break;
          case 7:
            cout << "case 7\n";
            s.push('#');
            break;
          case 77:
            cout << "case 77\n";
            s.push('(');
            s.push('E');
            s.push(')');
            break;
          case 777:
            cout << "case 777\n";
            s.push('G');
            s.push('V');
            s.push('V');
            break;
          case 8:
            cout << "case 8\n";
            s.push('+');
            break;
          case 88:
            cout << "case 88\n";
            s.push('-');
            break;
          case 888:
            cout << "case 888\n";
            s.push('0');
            break;
          case 9:
            cout << "case 9\n";
            s.push('1');
            break;
          case 90:
            cout << "case 90\n";
            s.push('2');
            break;
          case 999:
            cout << "case 999\n";
            s.push('3');
            break;
          case 9999:
            cout << "case 9999\n";
            s.push('4');
            break;
          case 99999:
            cout << "case 99999\n";
            s.push('5');
            break;
          case 999999:
            cout << "case 999999\n";
            s.push('6');
            break;
          case 9999999:
            cout << "case 9999999\n";
            s.push('7');
            break;
          case 99999999:
            cout << "case 99999999\n";
            s.push('8');
            break;
          case 999999999:
            cout << "case 999999999\n";
            s.push('9');
            break;
        }

        display(s);

        if (s.top() == y) {
          loop = 0;
                  std::cout << "-----MATCH: "  << s.top() << '\n';
          s.pop();
        }

      } while(loop != 1);
      //reset loop
      loop = 1;
    }

    return 0;
}

void display(std::stack<char> z) {
  int x, j = z.size();
  char holder[x] = {'0'};
  for (int i = 0; i < x; i++) {
    holder[j - 1] = z.top();
    j--;
    if (z.top() != '$') {
      z.pop();
    }
  }
  std::cout << holder << '\n';
}