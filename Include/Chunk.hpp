#ifndef CHUNK_H
#define CHUNK_H

#include <string>
#include <vector>
struct Chunk {
    int id;
    int startLine;
    int endLine;
    string text;
    string filepath;
    vector<float> embed;
    string language;
};

#endif