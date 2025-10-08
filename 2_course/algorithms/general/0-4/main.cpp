#include <iostream>
#include <vector>

using namespace std;

int req(vector<pair<int, int>> v, const int& index) {
    int term = v[index-1].first + v[index].second;
    if (index == v.size() - 1)
        return term;
    return max(term, req(v.erase(v.begin() + index), index + 1));
}

int main() {
    int s;
    cin >> s;
    vector<pair<int, int>> matrix(s);
    for (int i = 0; i < s; i++) {
        cin >> matrix[i].first;
        cin >> matrix[i].second;
    }

}