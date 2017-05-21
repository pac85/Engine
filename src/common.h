#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <list>
#include <stdexcept>
#include <functional>
#include <set>
#include <cstring>

using namespace std;
//using namespace glm;

extern char* work_dir;

//reads a binary file into a vector ( inspired from vulkan-tutorial.com
inline vector<char> bin_read_file(string filename)
{
    vector<char> temp;
    ifstream ifile;
    //strats from the end so seekg will be equal to the file's length
    ifile.open(filename, std::ios::ate | std::ios::binary);

    if (!ifile.is_open())
        throw std::runtime_error("failed to open file!");

    temp.resize(ifile.tellg());
    ifile.seekg(0);
    ifile.read(temp.data(), temp.size());

    ifile.close();
    return temp;
}

#endif // COMMON_H_INCLUDED
