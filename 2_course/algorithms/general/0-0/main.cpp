#include <iostream>
#include <set>
#include <fstream>

int main() {

    try {
        std::ifstream in("input.txt");
        int input;
        std::set<int> nodes;
        while (in >> input) {
            nodes.insert(input);
        }
        nodes.insert(input);
        in.close();

        __int64 total = 0;
        for (int node : nodes) {
            total += node;
        }
        std::ofstream out("output.txt");
        out << total;
        out.close();
    } catch (std::fstream::failure e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
