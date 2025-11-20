#include <fstream>
#include <iostream>
#include <vector>

int main() {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    int s, n, m;
    in >> s >> n >> m;

    long long* items = new long long[s + 1];
    long long* time = new long long[s + 1];
    items[0] = 0; time[0] = 0;
    for (int i = 1; i <= s; i++) {
        in >> items[i] >> time[i];
    }

    long long boxSize = n * m;

    long long** dp = new long long*[s + 1];
    for (int i = 0; i <= s; i++) {
        dp[i] = new long long[boxSize + 1];
    }
    for (int i = 0; i < boxSize + 1; i++) {
        dp[0][i] = 0;
    }

    for (int i = 1; i < s + 1; i++) {
        for (int j = 0; j < boxSize + 1; j++) {
            dp[i][j] = dp[i-1][j];
            if (j >= items[i] && (dp[i][j] < dp[i - 1][j - items[i]] + time[i])) {
                dp[i][j] = dp[i - 1][j - items[i]] + time[i];
            }
        }
    }

    for (int i = 1; i < s + 1; i++) {
        for (int j = 0; j < boxSize + 1; j++) {
            std::cout << dp[i][j] << " ";
        }
        std::cout << std::endl;
    }

    int best = 0;
    for (int j = 0; j <= boxSize; j++) {
        if (dp[s][j] > dp[s][best]) best = j;
    }

    std::vector<int> solution;
    int curr = best;
    for (int i = s; i >= 1; i--) {
        if (dp[i][curr] != dp[i-1][curr] && curr >= items[i]) {
            solution.push_back(i);
            curr -= items[i];
        }
    }

    out << solution.size() << '\n';
    for (int i = solution.size() - 1; i >= 0 ; i--) {
        out << solution[i] << ' ';
    }

    return 0;
}