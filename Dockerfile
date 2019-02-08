FROM ubuntu:xenial

ENV DEBIAN_FRONTEND noninteractive

# Development
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        cmake \
        curl \
        git \
        vim \
        wget

# OpenCV dependencies
RUN apt-get install -y --no-install-recommends \
    libboost-dev \
    libeigen3-dev

# Build OpenCV
RUN mkdir /tmp/opencv-build
WORKDIR /tmp/opencv-build

ENV OPENCV_VERSION="3.4.2"

RUN wget -O opencv_contrib.tar.gz https://github.com/opencv/opencv_contrib/archive/${OPENCV_VERSION}.tar.gz \
&& tar -xzf opencv_contrib.tar.gz \
&& rm -rf opencv_contrib-${OPENCV_VERSION}/modules/datasets

RUN wget https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.tar.gz \
&& tar -xzf ${OPENCV_VERSION}.tar.gz \
&& cd opencv-${OPENCV_VERSION} \
&& mkdir build \
&& cd build \
&& cmake \
  -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-${OPENCV_VERSION}/modules \
  -DBUILD_TIFF=ON \
  -DBUILD_opencv_java=OFF \
  -DWITH_CUDA=OFF \
  -DWITH_OPENGL=OFF \
  -DWITH_OPENCL=OFF \
  -DWITH_IPP=OFF \
  -DWITH_TBB=OFF \
  -DWITH_EIGEN=ON \
  -DWITH_V4L=OFF \
  -DBUILD_TESTS=OFF \
  -DBUILD_PERF_TESTS=OFF \
  -DCMAKE_BUILD_TYPE=RELEASE \
  .. \
&& make -j$(nproc) \
&& make install \
&& rm -rf /tmp/opencv-build

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        python3 \
        python3-pip \
        unzip

COPY requirements.txt /tmp/requirements.txt
RUN pip3 install -r /tmp/requirements.txt
