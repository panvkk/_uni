#include <fstream>
#include <vector>

using namespace std;

struct node {
    int data;
    node* left;
    node* right;
};

void leftBypass(ostream& out, const node* currentRoot) {
    out << currentRoot->data << '\n';
    if (currentRoot->left != nullptr) {
        leftBypass(out, currentRoot->left);
    }
    if (currentRoot->right != nullptr) {
        leftBypass(out, currentRoot->right);
    }
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    int value;
    in >> value;
    node bst(value, nullptr, nullptr);

    while (in >> value) {
        node* curr = &bst;

        while (true) {
            if (value > curr->data) {
                if (curr->right == nullptr) {
                    curr->right = new node(value, nullptr, nullptr);
                    break;
                }
                curr = curr->right;
            } else if ( value < curr->data) {
                if (curr->left == nullptr) {
                    curr->left = new node(value, nullptr, nullptr);
                    break;
                }
                curr = curr->left;

            } else {
                break;
            }
        }
    }

    leftBypass(out, &bst);
    return 0;
}