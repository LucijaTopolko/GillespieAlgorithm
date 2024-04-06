#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Node {
    string name;
    double length{};
    vector<Node> elements;
};

class NewickParser {
public:
    Node parse(const string& newickString) {
        size_t index = 0;
        return parseHelper(newickString, index);
    }

private:
    Node parseHelper(const string &newickString, size_t &index) {
        Node node;

        while (index < newickString.size()) {
            char currentChar = newickString[index];
            if (currentChar == '(') {
                index++;
                node.elements.push_back(parseHelper(newickString, index));
            } else if (currentChar == ')') {
                index++;
                string nodeName;
                while (index < newickString.size() && newickString[index] != ',' && newickString[index] != ')' && newickString[index] != '(' && newickString[index] != ':') {
                    nodeName += newickString[index];
                    index++;
                }
                index++;
                string length;
                while (index < newickString.size() && newickString[index] != ',' && newickString[index] != ')' && newickString[index] != '(') {
                    length += newickString[index];
                    index++;
                }
                node.name = nodeName;
                node.length = stod(length);
                return node;
            } else if (currentChar == ',' || currentChar == ':') {
                index++;
            } else {
                string nodeName;
                while (index < newickString.size() && newickString[index] != ',' && newickString[index] != ')' && newickString[index] != '(' && newickString[index] != ':') {
                    nodeName += newickString[index];
                    index++;
                }
                index++;
                string length;
                while (index < newickString.size() && newickString[index] != ',' && newickString[index] != ')' && newickString[index] != '(') {
                    length += newickString[index];
                    index++;
                }
                Node newNode;
                newNode.name = nodeName;
                newNode.length = stod(length);
                node.elements.push_back(newNode);
            }
        }
        return node;
    }
};

void dfs(const Node &node, int depth = 0)
{
    for (int i = 0; i < depth; ++i)
    {
        cout << "  ";
    }
    cout << node.name << " "<< node.length << endl;
    for (const auto &child : node.elements)
    {
        dfs(child, depth + 1);
    }
}


int main() {
    string tree;
    ifstream file("tree.nwk");
    getline(file, tree);
    file.close();

    NewickParser parser;
    Node root = parser.parse(tree);

    dfs(root.elements.front());

    return 0;
}
