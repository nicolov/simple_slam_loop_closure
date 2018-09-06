#pragma once

#include <DBoW2/DBoW2.h>

#include <opencv/cv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

namespace slc {
struct FrameDescriptor {
  /*
   * Computes a global representation for an image by using
   * the SURF feature descriptor in OpenCV and the bag of
   * words approach.
   */
  FrameDescriptor(const std::string& vocabulary_path) {
    std::cout << "Loading vocabulary from " << vocabulary_path << std::endl;
    vocab_.reset(new Surf64Vocabulary(vocabulary_path));
    std::cout << "Loaded vocabulary with " << vocab_->size() << " visual words."
              << std::endl;
  }

  void extract_surf(const cv::Mat& img, std::vector<cv::KeyPoint>& keys,
                    std::vector<std::vector<float>>& descriptors) {
    /* Extracts SURF interest points and their descriptors. */

    static cv::Ptr<cv::xfeatures2d::SURF> surf_detector =
        cv::xfeatures2d::SURF::create(400);

    surf_detector->setExtended(false);

    std::vector<float> plain;
    surf_detector->detectAndCompute(img, cv::Mat(), keys, plain);

    const int L = surf_detector->descriptorSize();
    descriptors.resize(plain.size() / L);

    unsigned int j = 0;
    for (unsigned int i = 0; i < plain.size(); i += L, ++j) {
      descriptors[j].resize(L);
      std::copy(plain.begin() + i, plain.begin() + i + L,
                descriptors[j].begin());
    }
  }

  void describe_frame(const cv::Mat& img, DBoW2::BowVector& bow_vec) {
    /* Transforms the feature descriptors to a BoW representation
     *  of the whole image. */

    std::vector<cv::KeyPoint> keypoints;
    std::vector<std::vector<float>> descriptors;

    extract_surf(img, keypoints, descriptors);
    vocab_->transform(descriptors, bow_vec);
  }

  std::unique_ptr<Surf64Vocabulary> vocab_;
};
}  // namespace slc
