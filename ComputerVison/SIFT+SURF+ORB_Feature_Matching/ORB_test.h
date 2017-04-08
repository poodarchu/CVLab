//
// Created by Poodar Chu on 4/8/17.
//

#ifndef OPENCV_BASICS_ORB_BRIEFT_TEST_H
#define OPENCV_BASICS_ORB_BRIEFT_TEST_H

#include <opencv2/core/core.hpp>
#include <iostream>
#include <ctime>

using namespace cv;
using namespace std;

void ORB_test(){

    // ratio test proposed by David Lowe paper = 0.8
    double ratio = atof("0.8");

    string image1_name = string("/Users/poodar/Developments/Cpp/ClionProjects/OpenCV_Basics/images/book.jpeg");
    string image2_name = string("/Users/poodar/Developments/Cpp/ClionProjects/OpenCV_Basics/images/book_scene.jpeg");

    Mat image1 = imread(image1_name, 1);
    Mat image2 = imread(image2_name, 1);

    Ptr<FeatureDetector> detector;
    Ptr<DescriptorExtractor> extractor;

    detector = FeatureDetector::create("ORB");
    extractor = DescriptorExtractor::create("ORB");

    clock_t begin = clock();

    vector<KeyPoint> keypoints1, keypoints2;
    detector->detect(image1, keypoints1);
    detector->detect(image2, keypoints2);

    cout << "# keypoints of image1 :" << keypoints1.size() << endl;
    cout << "# keypoints of image2 :" << keypoints2.size() << endl;

    Mat descriptors1,descriptors2;
    extractor->compute(image1,keypoints1,descriptors1);
    extractor->compute(image2,keypoints2,descriptors2);



    cout << "Descriptors size :" << descriptors1.cols << ":"<< descriptors1.rows << endl;

    vector< vector<DMatch> > matches12, matches21;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->knnMatch( descriptors1, descriptors2, matches12, 2 );
    matcher->knnMatch( descriptors2, descriptors1, matches21, 2 );

    // BFMatcher bfmatcher(NORM_L2, true);
    // vector<DMatch> matches;
    // bfmatcher.match(descriptors1, descriptors2, matches);
    cout << "Matches1-2:" << matches12.size() << endl;
    cout << "Matches2-1:" << matches21.size() << endl;

    std::vector<DMatch> good_matches1, good_matches2;

    for(int i=0; i < matches12.size(); i++){
        if(matches12[i][0].distance < ratio * matches12[i][1].distance)
            good_matches1.push_back(matches12[i][0]);
    }

    for(int i=0; i < matches21.size(); i++){
        if(matches21[i][0].distance < ratio * matches21[i][1].distance)
            good_matches2.push_back(matches21[i][0]);
    }

    cout << "Good matches1:" << good_matches1.size() << endl;
    cout << "Good matches2:" << good_matches2.size() << endl;

    // Symmetric Test
    std::vector<DMatch> better_matches;
    for(int i=0; i<good_matches1.size(); i++){
        for(int j=0; j<good_matches2.size(); j++){
            if(good_matches1[i].queryIdx == good_matches2[j].trainIdx && good_matches2[j].queryIdx == good_matches1[i].trainIdx){
                better_matches.push_back(DMatch(good_matches1[i].queryIdx, good_matches1[i].trainIdx, good_matches1[i].distance));
                break;
            }
        }
    }
    cout << "Better matches:" << better_matches.size() << endl;

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Time Costs : " << elapsed_secs << endl;

    Mat output;
    drawMatches(image1, keypoints1, image2, keypoints2, better_matches, output);
    imshow("Matches result",output);
    waitKey(0);

}

#endif //OPENCV_BASICS_ORB_BRIEFT_TEST_H
