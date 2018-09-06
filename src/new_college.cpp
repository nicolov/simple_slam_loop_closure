#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include <DBoW2/DBoW2.h>

#include "frame_descriptor.h"
#include "utils.h"

using namespace std;
using namespace slc;

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cerr << "Usage " << argv[0] << " [path_to_vocabulary] [path to images]"
         << endl;
    exit(1);
  }

  string vocabulary_path(argv[1]);
  FrameDescriptor descriptor(vocabulary_path);

  string dataset_folder(argv[2]);
  const auto filenames = load_filenames(dataset_folder);
  std::cout << "Processing " << filenames.size() << " images\n";

  // Will hold BoW representations for each frame
  vector<DBoW2::BowVector> bow_vecs;

  for (unsigned int img_i = 0; img_i < filenames.size(); img_i++) {
    auto img_filename = dataset_folder + "/Images/" + filenames[img_i];
    auto img = cv::imread(img_filename);

    cout << img_filename << endl;

    if (img.empty()) {
      cerr << endl << "Failed to load: " << img_filename << endl;
      exit(1);
    }

    // Get a BoW description of the current image
    DBoW2::BowVector bow_vec;
    descriptor.describe_frame(img, bow_vec);
    bow_vecs.push_back(bow_vec);
  }

  cout << "Writing output..." << endl;

  ofstream of;
  of.open(getenv("HOME") +
          string("/dev/simple_slam_loop_closure/out/confusion_matrix.txt"));

  // Compute confusion matrix
  // i.e. the (i, j) element of the matrix contains the distance
  // between the BoW representation of frames i and j
  for (unsigned int i = 0; i < bow_vecs.size(); i++) {
    for (unsigned int j = 0; j < bow_vecs.size(); j++) {
      of << descriptor.vocab_->score(bow_vecs[i], bow_vecs[j]) << " ";
    }
    of << "\n";
  }

  of.close();
  cout << "Output done" << endl;
}