#pragma once

#include <string>

enum class Mode : char {
    CREATE_NODES, CREATE_EDGES, DEFAULT
};

struct Settings {
    std::string image_path;
    std::string graph_path;
    Mode mode;
    bool load;
    bool save;
};

Settings parseOptions(int argc, char * argv[]);