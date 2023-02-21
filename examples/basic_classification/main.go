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
	labelsFile = "/CaffeNet.labels"
)

func main() {

	// Read in the image file name.
	args := os.Args
	if len(args) != 2 {
		log.Fatal("Please provide an image as input")
	}
	imageFile := args[1]

	// Create an inference configurator value.
	pluginPaths := dlinfer.NewStringVector()
	pluginPaths.Add(pluginPath)
	configurator := dlinfer.NewInferenceEngineConfigurator(model, pluginPaths, plugin, labelsFile)

	// L