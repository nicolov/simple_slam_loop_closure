# Simple loop closure for Visual SLAM

Possibily the simplest example of loop closure for Visual SLAM. More
information [on my blog](http://nicolovaligi.com/bag-of-words-loop-closure-
visual-slam.html).

As I'm experimenting with alternative approaches for SLAM loop closure, I
wanted a baseline that was reasonably close to state-of-the art approaches.
The approach here is pretty similar to ORB-SLAM's, and uses SURF descriptors
and *bag of words* to translate them to a global image description vector.

## The dataset

For testing, I've used the New College dataset published alongside FAB-MAP.
It's available for download
[here](http://www.ijrr.org/ijrr_2008/volume27-issue6/090961/3_data.htm). It's
ideal for loop-closure testing, since it includes manual place associations
that can be used for evaluation.

## Building with Docker

You can build and run the code using `docker-compose` and Docker. The Docker
configuration uses a Ubuntu 16.04 base image, and builds OpenCV 3 from source.

```
# Will take ~10 minutes to download and build OpenCV 3
docker-compose build runner
docker-compose run runner bash

# You're now in a shell inside the Docker container:
mkdir build
cd build
cmake ..
make -j$(nproc)

./new_college
```

## Compatibility

Only tested on Ubuntu 16.04 LTS with OpenCV3, gcc 5.4.0

## Plotting the confusion matrix

The `ground_truth_comparison.py` plots and compares the loop closures from the
ground truth to the actual results from the code.
