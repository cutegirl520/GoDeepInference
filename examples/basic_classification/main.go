package main

import (
	"log"
	"os"

	"github.com/gopherds/dlinfer"
)

const (
	model      = "/CaffeNet.xml"
	pluginPath = "/opt/intel/deep_learning_sdk_2016.1.0.861/deployment_tools/inference_engine/lib/intel64/"
	plugin     = "MKLDNNPlugin"
	labelsFile = "/CaffeN