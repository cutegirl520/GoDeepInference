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
 * \brief Declaration of methods and classes for working with Inference Engine API
 * \file InferenceEngineConfigurator.h
 * \example inference_engine_classification_sample/core/InferenceEngineConfigurator.h
 */
#pragma once

#include <string>
#include <vector>
#include <ie_so_loader.h>
#include <ie_cnn_net_reader.h>
#include <inference_engine.hpp>
#include <ie_plugin_ptr.hpp>

class LabelProbability {
private:
	/// Index of current label
	int labelIdx = 0;
	/// Name of class from file with labels
	std::string className;
	/// The probability of prediction
	float probability = 0.0f;

public:
	/**
	 * Constructor of InferenceResults class
	 * @param labelIdx - index of current label
	 * @param probability - the probability of prediction
	 * @param className - name of class from file with labels
	 * @return InferenceResults object
	 */
	LabelProbability(int labelIdx, float probability, std::string className) : labelIdx(labelIdx),
		                                                                   className(className),
		                                                                   probability(probability) {}

	/**
	 * Get label index
	 * @return index of current label
	 */
	const int &getLabelIndex() const {
	    return labelIdx;
	}

	/**
	 * Get label name
	 * @return label
	 */
	const std::string &getLabel() const {
	    return className;
	}

	/**
	 * Get probability
	 * @return probability
	 */
	const float &getProbability() const {
	    return probability;
	}
};

/**
* \class InferenceResults
* \brief Represents predicted data in easy to use format
*/
class InferenceResults {
private:
	std::string image;
	std::vector<LabelProbability> results;

public:
	explicit InferenceResults(std::string &name) {
	    image = name;
	}

	void addResult(LabelProbability result) {
	    results.push_back(result);
	}

	const std::string &getName() const {
	    return image;
	}

	const std::vector<LabelProbability> &getResults() const {
	    return results;
	}
};

/**
 * \class InferenceEngineConfigurator
 * \brief This class communicates with Inference Engine
 */
class InferenceEngineConfigurator {
public:
    /**
     * Constructor of InferenceEngineConfigurator class
     * @param modelFile - the path to model in .xml format
     * @param pluginPath - the path to plugin
     * @param pluginName - the name of plugin for prediction
     * @param la