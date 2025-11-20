#include <fstream>
#include <algorithm>

using namespace std;

struct node {
    int data;
    node* left;
    node* right;

    node(int val) : data(val), left(nullptr), right(nullptr) {}
    node(int val, node* l, node* r) : data(val), left(l), right(r) {}
};

void leftBypass(ostream& out, const node* currentRoot) {
    if (currentRoot == nullptr) return;
    out << currentRoot->data << '\n';
    leftBypass(out, currentRoot->left);
    leftBypass(out, currentRoot->right);
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

int maxPathLen = -1;
int minDepthOfRoot = 1000000;
node* nodeToDelete = nullptr;

int findLongestSemiPath(node* curr, int depth) {
    if (curr == nullptr) {
        return 0;
    }
    int leftHeight = findLongestSemiPath(curr->left, depth + 1);
    int rightHeight = findLongestSemiPath(curr->right, depth + 1);
    int currentPathLen = leftHeight + rightHeight;

    if (currentPathLen > maxPathLen) {
        maxPathLen = currentPathLen;
        minDepthOfRoot = depth;
        nodeToDelete = curr;
    } else if (currentPathLen == maxPathLen) {
        if (depth < minDepthOfRoot) {
            minDepthOfRoot = depth;
            nodeToDelete = curr;
        }
    }

    return max(leftHeight, rightHeight) + 1;
}

int main() {
    ifstream in("in.txt");
    ofstream out("out.txt");

    int value;
    in >> value;
    node* bst = nullptr;
    bst = new node(value);
    while (in >> value) {
        node* curr = bst;
        node* newNode = new node(value);

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
                delete newNode; break;
            }
        }
    }
    findLongestSemiPath(bst, 0);

    if (nodeToDelete != nullptr) {
        node* father = nullptr;
        node* target = nullptr;

        findTarget(bst, nodeToDelete->data, target, father);

        deleteNode(father, target, bst);
    }
    leftBypass(out, bst);

    in.close();
    out.close();

    return 0;
}