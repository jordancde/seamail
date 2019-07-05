
#include <iostream>

using namespace std;

int main() {
    if(rand() % 2 == 0){
        cout << "pass!" << endl;
        return 0;
    } else {
        cout << "fail!" << endl;
        return 1;
    }    
}
