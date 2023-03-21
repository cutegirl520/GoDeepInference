// All material is licensed under the Apache License Version 2.0, January 2004
// http://www.apache.org/licenses/LICENSE-2.0

package dlinfer

import "os"

// #cgo CXXFLAGS: -std=c++11 -I/usr/include -I/opt/intel/deep_learning_sdk_2016.1.0.861/deployment_tools/inference_engine/include -I/opt/intel/deep_learning_sdk_2016.1.0.861/deployment_tools/inference_engine/samples/format_reader
// #cgo LDFLAGS: -L/opt/intel/deep_learning_sdk_2016.1.0.861/deployment_tools/inference_engine/bin/intel64/lib 