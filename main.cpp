#include <iostream>
#include <vector>

using namespace std;

int autoTest();

int main(int argc, char **argv) {
    std::cout << "Hello, World ,good to see u!" << std::endl;
    //autoTest();
    return 0;
}


int autoTest() {
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    for (auto i : v) {
        cout << i << " ";
    }
    cout << endl;

    return 0;

}
