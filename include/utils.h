#pragma once

#include <vector>

std::vector<std::string> load_filenames(const std::string& folder, const bool skip_even = true) {
    std::vector<std::string> filenames;

    auto index_filename = folder + "/ImageCollectionCoordinates.txt";

    std::cout << "Opening " << index_filename << "\n";

    std::ifstream f;
    f.open(index_filename);

    while (!f.eof()) {
        std::string l;
        getline(f, l);
        
        if (!l.empty()) {
            std::stringstream ss;
            ss << l;
            std::string filename;
            ss >> filename;
            filenames.push_back(filename);
        }

        // Discard even-numbered images (for stereo datasets)
        if (skip_even) {
            std::getline(f, l);
        }
    }

    return filenames;
}