#include <fstream>
#include <queue>

long long getMin(std::queue<long long>& q1, std::queue<long long>& q2) {
    if (q2.empty() || (!q1.empty() && q1.front() <= q2.front())) {
        long long min = q1.front();
        q1.pop();
        return min;
    }
    long long min = q2.front();
    q2.pop();
    return min;
}

int main() {
    std::ifstream input("huffman.in");
    std::ofstream output("huffman.out");

    int n;
    input >> n;
    std::queue<long long> q1, q2;

    int val;
    for (int i = 0; i < n; i++) {
        input >> val;
        q1.push(val);
    }

    long long min1, min2;
    long long answer = 0;
    while (q1.size() + q2.size() > 1) {
        min1 = getMin(q1, q2);
        min2 = getMin(q1, q2);

        long long newVer = min1 + min2;
        q2.push(newVer);
        answer += newVer;
    }

    output << answer << std::endl;
    return 0;
}