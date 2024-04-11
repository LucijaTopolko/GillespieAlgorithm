//
// Created by Lucija Topolko on 06/04/2024.
//

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Node {
    string name;
    double length{};
    string sequence;
    string parentSequence;
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

Node parseTree(const string& filename) {
    string tree;
    ifstream file(filename);
    getline(file, tree);
    file.close();

    NewickParser parser;
    Node root = parser.parse(tree);

    return root.elements.front();
}

#endif // PARSE_TREE_H
