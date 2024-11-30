//
// Created by Ramesh Kumar on 13/11/24.
//

#include <iostream>
#include <filesystem>
#include <fstream>

#include "include/helpers.h"

namespace fs = std::filesystem;

std::vector<std::string> helper::load_dir_images(const std::string & folderPath) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        const fs::path & _path = entry.path();
        if (const fs::path& extension = _path.extension(); extension == ".jpg" || extension == ".png" || extension == ".jpeg") {
            files.emplace(files.begin(), _path.string());
        }
    }
    return files;
}

std::vector<std::string> helper::load_class_list(const std::string & filePath) {

    std::vector<std::string> class_list;
    std::ifstream if_stream (filePath);

    if (!if_stream.is_open()) {
        std::cerr << "Error opening file" << std::endl;
    }

    std::string line;
    while (std::getline(if_stream, line)) {
        class_list.push_back(line);
    }
    return class_list;
}

