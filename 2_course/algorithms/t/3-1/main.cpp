#include <iostream>
#include <string>

using namespace std;

bool is_palindrome(const string& s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) return false;
        left++;
        right--;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;

    int n = s.length();

    int front_a = 0;
    while (front_a < n && s[front_a] == 'a') {
        front_a++;
    }

    if (front_a == n) {
        cout << "Yes" << endl;
        return 0;
    }

    int back_a = 0;
    while (n - 1 - back_a >= 0 && s[n - 1 - back_a] == 'a') {
        back_a++;
    }

    if (front_a > back_a) {
        cout << "No" << endl;
    } else {
        if (is_palindrome(s, front_a, n - 1 - back_a)) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
    }

    return 0;
}