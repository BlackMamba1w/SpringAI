#ifndef CHUNK_H
#define CHUNK_H

#include <string>
#include <vector>
struct Chunk {
    int id;
    int startPos;
    int endPos;
    string text;
    string source;
    vector<float> embed;
    string language;
};

#endif