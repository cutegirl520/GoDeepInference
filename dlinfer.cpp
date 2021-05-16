
/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

/**
 * \brief Implementations of methods for working with Inference Engine API
 * \file InferenceEngineConfigurator.cpp
 * \example inference_engine_classification_sample/core/InferenceEngineConfigurator.cpp
 */
#include "dlinfer.h"
#include <format_reader_ptr.h>
#include <ie_plugin.hpp>
#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <stdarg.h>

using namespace InferenceEngine;

static std::string fileNameNoExt(const std::string &filepath) {
    auto pos = filepath.rfind('.');
    if (pos == std::string::npos) return filepath;
    return filepath.substr(0, pos);
}

// trim from both ends (in place)
inline std::string &trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

static inline std::string stringFormat(const char *msg, ...) {
    va_list va;
    va_start(va, msg);
    char buffer[65536];

    vsnprintf(buffer, sizeof(buffer), msg, va);
    va_end(va);
    return buffer;
}


InferenceEngineConfigurator::InferenceEngineConfigurator(const std::string &modelFile,
                                                         const std::vector<std::string> &pluginPath,
                                                         const std::string &pluginName, const std::string &labelFile)
        : _plugin(selectPlugin(pluginPath, pluginName)) /* connect to plugin */, imageLoaded(false) {
    // Create network reader and load it from file
    network.ReadNetwork(modelFile);
    if (!network.isParseSuccess()) THROW_IE_EXCEPTION << "cannot load a failed Model";
    _plugin->Unload();

    // Get file names for files with weights and labels
    std::string binFileName = fileNameNoExt(modelFile) + ".bin";
    network.ReadWeights(binFileName.c_str());

    std::string labelFileName = fileNameNoExt(modelFile) + ".labels";

    // Change path to labels file if necessary
    if (!labelFile.empty()) {
        labelFileName = labelFile;
    }

    // Try to read labels file
    readLabels(labelFileName);
}

/*
 * Method reads labels file
 * @param fileName - the file path
 * @return true if all success else false
 */
bool InferenceEngineConfigurator::readLabels(const std::string &fileName) {
    _classes.clear();

    std::ifstream inputFile;
    inputFile.open(fileName, std::ios::in);
    if (!inputFile.is_open())
        return false;

    std::string strLine;
    while (std::getline(inputFile, strLine)) {
        trim(strLine);
        _classes.push_back(strLine);
    }

    return true;
}

void InferenceEngineConfigurator::loadImages(const std::string &image) {
    std::vector<std::string> imageVector;
    imageVector.push_back(image);
    loadImages(imageVector);
}

void InferenceEngineConfigurator::loadImages(const std::vector<std::string> &images) {
    InferenceEngine::SizeVector inputDims;
    network.getInputDimentions(inputDims);
    size_t batchSize = inputDims.at(inputDims.size() - 1);
    inputDims.at(inputDims.size() - 1) = 1;

    int inputNetworkSize = std::accumulate(inputDims.begin(), inputDims.end(), 1, std::multiplies<size_t>());

    if (!inputDims.size()) {
        THROW_IE_EXCEPTION << "Error: Incorrect network input dimensions!";
    }

    std::vector<std::shared_ptr<unsigned char>> readImages;

    for (auto i = 0; i < images.size(); i++) {
        FormatReader::ReaderPtr reader(images.at(i).c_str());
        if (reader.get() == nullptr) {
            std::cerr << "[WARNING]: Image " << images.at(i) << " cannot be read!" << std::endl;
            continue;
        }
        if (reader->size() != inputNetworkSize) {
            std::cerr << "[WARNING]: Input sizes mismatch, got " << reader->size() << " bytes, expecting "
                      << inputNetworkSize << std::endl;
            continue;
        }
        readImages.push_back(reader->getData());
        imageNames.push_back(images.at(i));
    }

    if (readImages.size() == 0) {
        THROW_IE_EXCEPTION << "Valid input images were not found!";
    }

    if (batchSize == 1) {
        network.getNetwork().setBatchSize(readImages.size());
    } else {
        if (batchSize > readImages.size()) {
            auto readImagesSize = readImages.size();
            size_t diff = batchSize / readImagesSize;

            for (auto i = 1; i < diff; i++) {
                for (auto j = 0; j < readImagesSize; j++) {
                    imageNames.push_back(imageNames.at(j));
                    readImages.push_back(readImages.at(j));
                }
            }
            if (readImagesSize * diff != batchSize) {
                for (auto j = 0; j < batchSize - readImagesSize * diff; j++) {
                    imageNames.push_back(imageNames.at(j));
                    readImages.push_back(readImages.at(j));
                }
            }
        } else if (batchSize < readImages.size()) {
            while (readImages.size() != batchSize) {