#include <fstream>
#include <stack>

using namespace std;

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    int n;
    in >> n;
    stack<int> s;

    float c1, c2;
    int topHose;
    while (in >> c1 >> c2 >> topHose) {
        if (!s.empty() && s.top() == topHose) {
            s.pop();
        } else {
            s.push(topHose);
        }
    }
    if (s.empty()) {
        out << "Yes\n";
    } else {
        out << "No\n";
    }
}