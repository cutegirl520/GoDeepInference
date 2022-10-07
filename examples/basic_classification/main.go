package main

import (
	"log"
	"os"

	"github.com/gopherds/dlinfer"
)

const (
	model      = "/CaffeNet.xml"
	pluginPath = "/opt/intel/deep_learning_sdk_2016.1.0.861/deployment_tools/