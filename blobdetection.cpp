#include "blobdetection.h"
#include "morphops.h"
#include "bloblibproc.h"
#include <lightfieldcorrector.h>
#include <fstream>
#include <string>
//#include <typeinfo>

BlobDetection::BlobDetection(){
    params.thresholdStep = 5.0f;
    params.minThreshold = 1.0f;
    params.maxThreshold = 150.0f;
    params.minDistBetweenBlobs = 35.0f;
    params.filterByInertia = false;
    params.filterByConvexity = false;
    params.filterByColor = false;
    params.blobColor = 255;
    params.filterByCircularity = false;
    params.filterByArea = true;
    params.minArea = 25.0f;
    params.maxArea = 80000.0;
}

void BlobDetection::runMorphs(ImageList<Image> &initIn){
    _procMorphs.convertProc(initIn);
    _procMorphs.threshProc();
    setBlobList(_procMorphs.getThreshList());
//    setBlobList(_procMorphs.getConvList());

}

void BlobDetection::setBlobList(ImageList<Image> inBlobList){
    _blobImList = inBlobList;
}

void BlobDetection::setBlobUnc(ImageList<Image> inRaw){
    _blobUnc = inRaw;
}

void BlobDetection::runDetect(/*ImageList<Image> in*/){
//    setBlobList(in);
    cv::SimpleBlobDetector detector(params);
    for(int i = 0; i < _blobImList.size(); i++){
        std::vector<cv::KeyPoint> keypoints;
        detector.detect(_blobImList.getImage(i).getBitmap(), keypoints);
        _blobHolder.push_back(keypoints);
        std::cout<<"Blob detection on Image: " << i << std::endl;
    };
}

ImageList<Image> BlobDetection::relayGetMorphConv(){
    return _procMorphs.getConvList();
}

ImageList<Image> BlobDetection::relayGetMorphThresh(){
    ImageList<Image> threshRelay = _procMorphs.getThreshList();
    return threshRelay;
}

void BlobDetection::statBlobs(){
    std::vector<std::string> statsVec;
    for(int i=0; i < _blobHolder.size(); i++){
        for(int j=0; j < _blobHolder[i].size(); j++ ){
            std::ostringstream stats;
            std::string tstamp = _blobImList[i].getName();
            tstamp =tstamp.substr(5,26);
            stats << tstamp << ","<< j+1 << "," <<  _blobHolder[i][j].size << "," << _blobHolder[i][j].pt <<std::endl;
//            stats << _blobImList[i].getName() << ","<< j+1 << "," <<  _blobHolder[i][j].size << "," <<std::endl;
//            stats << "Image," << _blobImList[i].getName() << ",Keypoint,"<< j+1 << ",Coordinates," << _blobHolder[i][j].pt.x << "," <<  _blobHolder[i][j].pt.y << ",Size," <<  _blobHolder[i][j].size << "," <<std::endl;
//            stats << "Image #: " << i +1 << "\n Keypoint #: "<< j+1 << "\n Coordinates: " << _blobHolder[i][j].pt.x << ", " <<  _blobHolder[i][j].pt.y << "\n Keypoint Size: " <<  _blobHolder[i][j].size << "\n" <<std::endl;
            std::string statsString = stats.str();
            std::cout << statsString;
            statsVec.push_back(statsString);
        }
    }
    saveStats(statsVec);
}

void BlobDetection::saveStats(std::vector<std::string> stats){

    std::ofstream file;
    std::string imageName = _blobImList[0].getName();
    std::string p = "//home//bgrassian//ROIProc Output//";
    imageName =p + "Start: " + imageName.substr(5,26) + ".txt";
    file.open(imageName);
    for(int i=0; i < stats.size(); i++){
        file << stats[i];
    }
//    file.close();
    std::cout <<"Blob stats saved." << std::endl;
}


void BlobDetection::drawBlobs(){
    std::cout << "Start draw blobs:" << std::endl;
    ImageList<Image> rawIms = _procMorphs.getConvList();
    _drawnIm = rawIms;
    for(int z=0; z<_blobHolder.size(); z++){
        cv::Mat tempIn = rawIms.getImage(z).getBitmap();
        cv::Mat out;        
        std::vector<cv::KeyPoint> tempV = _blobHolder[z];
        cv::drawKeypoints(tempIn, tempV, out, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        _drawnIm.getImage(z).getBitmap()=out;
        std::cout <<"Blobs drawn on image " << z << std::endl;
    }
}


void BlobDetection::extractBlobs(){
    std::cout << "blob extraction" << std::endl;
//    ImageList<Image> convList = _procMorphs.getConvList();
    for(int i=0; i < _blobHolder.size(); i++){
//        cv::Mat blobCrop = convList.getImage(i).getBitmap();
        cv::Mat blobCrop = _blobUnc.getImage(i).getBitmap();
        ImageList<Image> tempVec;
//        std::string time = convList[i].getName();
         std::string time = _blobUnc[i].getName();
        time =time.substr(5,26);
        for(int j=0; j < _blobHolder[i].size(); j++ ){
            float tempSize = _blobHolder[i][j].size;
            int intSize = tempSize + 40;
            cv::Size rendSize;
            rendSize.height = intSize;
            rendSize.width = intSize;
            cv::Point2f tempPt = _blobHolder[i][j].pt;
            cv::Mat tempCrop;
            cv::getRectSubPix(blobCrop,rendSize, tempPt,tempCrop);
            Image tempIm;
            tempIm.getBitmap()=tempCrop;
            std::ostringstream name;
            name << "I_" << i+1 << " K_" << j+1 << " T_" << time <<".tif";
            std::string nameStr = name.str();
            tempIm.getFilePath() = nameStr;
            tempVec.pushBack(tempIm);
        }
        _extractedB.push_back(tempVec);
    }
}

void BlobDetection::saveExtractBlobs(){
    std::string repPath = "/home/bgrassian/ROIProc Output/2_Blob Directory/";
    for(int i =0; i < _extractedB.size(); i++){
        std::string strI = std::to_string(i);
        std::string pathString = repPath +"Image "+  strI + "/";
        boost::filesystem::path p = pathString ;
        boost::filesystem::create_directories(p);
        _extractedB[i].save(repPath + "Image " + strI + "/");
        std::cout <<"Saved blob batch on image " << i << std::endl;
    }
}

ImageList<Image> BlobDetection::getDrawnBlobs(){
    return(_drawnIm);
}


void BlobDetection::saveDrawnBlobs(){
    std::cout << "Saving blob-overlayed images.. : " << std::endl;
    _drawnIm.save("/home/bgrassian/ROIProc Output/1_Drawn Blobs");
    std::cout << "Blob overlayed images saved." << std::endl;
}


void BlobDetection::load16BitRaw(){
    boost::filesystem::path pa("/home/bgrassian/filter");
//    ImageList<Image> inList;
    boost::filesystem::directory_iterator end_itr;
    int numbIm=0;
    for (boost::filesystem::directory_iterator itr(pa); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path())) {
            std::string current_file = itr->path().string();
            std::string ext =itr-> path().extension().string();
            boost::algorithm::to_lower(ext);
            Image temp;
            cv::Mat tempa = cv::imread(current_file, 0);
            temp.getFilePath()=current_file;
            temp.getBitmap() = tempa;
            _blobUnc.pushBack(temp);
            numbIm++;
            std::cout<<"Loaded image #: " <<numbIm <<std::endl;
        }
    }
}



// prior to 9_20
//BlobDetection::BlobDetection(){
//    params.thresholdStep = 5.0f;
//    params.minThreshold = 1.0f;
//    params.maxThreshold = 150.0f;
//    params.minDistBetweenBlobs = 15.0f;
//    params.filterByInertia = false;
//    params.filterByConvexity = false;
//    params.filterByColor = false;
//    params.blobColor = 255;
//    params.filterByCircularity = false;
//    params.filterByArea = true;
//    params.minArea = 25.0f;
//    params.maxArea = 80000.0;
//}
