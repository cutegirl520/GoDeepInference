# Go Deep Learning Inference Engine

This project aims to make trained models such as AlexNet, GoogleNet, etc., easily accessible within Go. It leverages the features of [Intel's inference engine](https://software.intel.com/en-us/deep-learning-sdk) (a part of the deep learning SDK) to interact with optimized and well-trained neural networks.

_Note_: This is a work in progress.

# Requirements

- [Go 1.8+](https://golang.org/)
- Ubuntu 14.04
- [Intel's Deep Learning SDK Deployment Tools](https://software.intel.com/en-us/deep-learning-sdk)
- [swig 3.0.6+](http://www.swig.org/)

# Usage

- To get the GoDeepInference package:

    ```
    go get github.com/cutegirl520/GoDeepInference
    ```

- Declare the following environmental variable:

    ```
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/intel/deep_learning_sdk_2016.1.0.861/deployment_tools/inference_engine/bin/intel64/lib:/opt/intel/deep_learning_sdk_2016.1.0.861/deployment_tools/inference_engine/lib/intel64
    ```

- You can build/install your Go progams as usual with `go build` and `go