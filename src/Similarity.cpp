#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
#include <vector>
#include "Configs.hpp"
#include "ChatStatus.hpp"
#include <cmath>            
#include <stdexcept> 
using json = nlohmann::json;
using namespace std;
float similarity(const vector<float>& A, const vector<float>& B){
    if (A.size() != B.size()){
        throw runtime_error("Vector dimensions do not match.")
    }
    float dot_product;
    float norm_A;
    float norm_B;
    for (size_t i = 0; i < A.size(); i++){
        dot_product += A[i] * B[i];
        norm_A += A[i];
        norm_B += B[i];
    }
    return dot_product/(sqrt(norm_A)*sqrt(norm_B));
}