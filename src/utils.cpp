//
// Created by Ramesh Kumar on 13/11/24.
//

#include <iostream>
#include <filesystem>

#include "include/utils.h"
#include "include/cimg.h"

namespace fs = std::filesystem;

std::vector<std::string> utils::load_dir_images(const std::string & folderPath) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        const fs::path & _path = entry.path();
        if (const fs::path& extension = _path.extension(); extension == ".jpg" || extension == ".png" || extension == ".jpeg") {
            files.emplace(files.begin(), _path.string());
        }
    }
    return files;
}

