#include "bloblibproc.h"
#include "blobdetection.h"
#include "morphops.h"
#include <iostream>

BlobLibProc::BlobLibProc(){
}

void BlobLibProc::loadBlobDir(){
    boost::filesystem::path p("/home/bgrassian/ROIProc Output/2_Blob Directory/");
    boost::filesystem::recursive_directory_iterator end_itr;
    int numbIm=0;
    for (boost::filesystem::recursive_directory_iterator itr(p); itr != end_itr; ++itr){
        if (is_regular_file(itr->path())) {
            std::string current_file = itr->path().string();
            std::string ext =itr-> path().extension().string();
            boost::algorithm::to_lower(ext);
            Image temp;
            cv::Mat tempa = cv::imread(current_file);
            temp.getFilePath()=current_file;
            temp.getBitmap() = tempa;
            _blobDir.pushBack(temp);
            numbIm++;
            std::cout<<"Loaded blob: " <<numbIm <<std::endl;
        }
    }
}

void BlobLibProc::cullBlobs(){
    double metric = 2000;
    for( int i=0; i <_blobDir.size() ; i++){
        cv::Mat te =_blobDir[i].getBitmap();
        cv::Scalar se = cv::sum(te);
        if( se.val[0] > metric){
            Image surv;
            surv.getBitmap()=te;
            surv.getFilePath() = _blobDir[i].getName();
            _blobSurvive.pushBack(surv);
        }
    }
    std::cout <<_blobSurvive.size() << std::endl;
    _blobSurvive.save("/home/bgrassian/ROIProc Output/6_Blobs Survived/");

}


void BlobLibProc::saveSurvive(){
//    boost::filesystem::path repPath = "/home/bgrassian/ROIProc Output/6_Blobs Survived/";
//    for(int i =0; i < _blobSurvive.size(); i++){
        _blobSurvive.save("/home/bgrassian/ROIProc Output/6_Blobs Survived/");

//    }
}


//std::string strI = std::to_string(i);
//std::string pathString = repPath +"Image "+  strI + "/";
//boost::filesystem::path p = pathString ;
//boost::filesystem::create_directories(p);
//_extractedB[i].save(repPath + "Image " + strI + "/");
//std::cout <<"Saved blob batch on image " << i << std::endl;

