#ifndef BLOBDETECTION_H
#define BLOBDETECTION_H
#include <lightfieldcorrector.h>
#include "morphops.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


class BlobDetection
{
public:
    BlobDetection();
    void runDetect(/*ImageList<Image> in*/);
    void statBlobs();
    void saveStats(std::vector<std::string>);
    void drawBlobs();
    void saveDrawnBlobs();
    ImageList<Image> getDrawnBlobs();
    void runMorphs(ImageList<Image> &initIn);
    void extractBlobs();
    void saveExtractBlobs();
    ImageList<Image> relayGetMorphConv();
    ImageList<Image> relayGetMorphThresh();
    void setBlobUnc(ImageList<Image> inRaw);
    void load16BitRaw();



private:
    void setBlobList(ImageList<Image> inBlobList);
    cv::SimpleBlobDetector::Params params;
    ImageList<Image> _blobImList;
    std::vector<std::vector<cv::KeyPoint> > _blobHolder;
    ImageList<Image> _drawnIm;
    ImageList<Image> _blobUnc;
    MorphOps _procMorphs;
//    std::vector<std::vector<cv::Mat> > extractedB;
    std::vector<ImageList<Image> > _extractedB;

};

#endif // BLOBDETECTION_H
