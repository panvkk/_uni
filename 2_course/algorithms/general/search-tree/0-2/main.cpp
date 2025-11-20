#include <fstream>

using namespace std;

struct node {
    int data;
    node *left;
    node *right;
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

void deleteNode(node*& father, node*& target, node*& bst) {
    if (father == nullptr) {
        if (target->left == nullptr && target->right == nullptr) {
            bst = nullptr;
        } else if (target->left == nullptr) {
            bst = target->right;
        } else if (target->right == nullptr) {
            bst = target->left;
        } else {
            node* prev = target;
            node* curr = target->right;
            while (curr->left != nullptr) {
                prev = curr;
                curr = curr->left;
            }
            target->data = curr->data;
            if (prev == target) {
                prev->right = curr->right;
            } else {
                prev->left = curr->right;
            }
        }
    } else {
        bool side = father->data < target->data;
        if (target->left == nullptr && target->right == nullptr) {
            if (side) {
                father->right = nullptr;
            } else {
                father->left = nullptr;
            }
        } else if (target->left == nullptr) {
            if (side) {
                father->right = target->right;
            } else {
                father->left = target->right;
            }
        } else if (target->right == nullptr) {
            if (side) {
                father->right = target->left;
            } else {
                father->left = target->left;
            }
        } else {
            node* prev = target;
            node* curr = target->right;
            while (curr->left != nullptr) {
                prev = curr;
                curr = curr->left;
            }
            target->data = curr->data;
            if (prev == target) {
                prev->right = curr->right;
            } else {
                prev->left = curr->right;
            }
        }
    }
}

void findTarget(node* bst, int targetValue, node*& target, node*& father) {
    node* prev = nullptr;
    node* curr = bst;
    while (curr != nullptr && curr->data != targetValue) {
        if (curr->data < targetValue) {
            prev = curr;
            curr = curr->right;
        } else if (curr->data > targetValue) {
            prev = curr;
            curr = curr->left;
        }
    }
    father = prev;
    target = curr;
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    int targetValue;
    in >> targetValue;
    int value;
    in >> value;
    node* bst = new node(value, nullptr, nullptr);

    while (in >> value) {
        node* curr = bst;
        node* newNode = new node(value, nullptr, nullptr);

        while (true) {
            if (value > curr->data) {
                if (curr->right == nullptr) {
                    curr->right = newNode;
                    break;
                }
                curr = curr->right;
            } else if (value < curr->data) {
                if (curr->left == nullptr) {
                    curr->left = newNode;
                    break;
                }
                curr = curr->left;
            } else {
                break;
            }
        }
    }

    node* father;
    node* target;
    findTarget(bst, targetValue, target, father);

    deleteNode(father, target, bst);
    leftBypass(out, bst);

    return 0;
}