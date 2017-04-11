#include "contourassembler.h"
//#include "blobdetection.h"
#include "morphops.h"
#include <lightfieldcorrector.h>
#include <iostream>

ContourAssembler::ContourAssembler()
{

}


void ContourAssembler::runMorphOps(ImageList<Image> &startList){
    _morphGetter.convertProc(startList);
//    setImages(_morphGetter.getConvList());
    //changed 9-26 for void thresh
//   setImages(_morphGetter.threshProc());
}

void ContourAssembler::setImages(ImageList<Image> in){
    _contImages = in;
}

ImageList<Image> ContourAssembler::getInList(){
    return _contImages;
}

void ContourAssembler::relayGetConv(){
    ImageList<Image> temp = _morphGetter.getConvList();
    setImages(temp);
}

//Place in MorphOps Class
void ContourAssembler::findEdges(/*ImageList<Image> inList*/){
    ImageList<Image> contOut = _contImages;
    
    for(int i=0; i < _contImages.size(); i++){
        cv::Mat tempIn = _contImages.getImage(i).getBitmap();
        cv::Mat tempOut;
        cv::Canny(tempIn, tempOut, 20,200);
        _contImages.getImage(i).getBitmap()= tempOut;
    }   
}

void ContourAssembler::findConts(){
    for(int i=0; i < _contImages.size(); i++){
        std::vector<cv::Vec4i> tempHierarchy;
        cv::Mat temp = _contImages.getImage(i).getBitmap();
        std::vector<std::vector<cv::Point> > contTemp;
//        cv::Mat out = temp;
        cv::findContours(temp, contTemp, tempHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
        _contourHolder.push_back(contTemp);
        _hierarchyHolder.push_back(tempHierarchy);
        std::cout << "Contours detected on Image " << _contImages[i].getName() <<std::endl;
    }
} 


void ContourAssembler::drawContours(){
    for(int i=0; i < _contImages.size(); i++){

        cv::Mat tempDraw = cv::Mat::zeros(_contImages[i].getBitmap().size(), _contImages[i].getBitmap().type());
        Image tempOut;

        for(int j=0; j < _contourHolder[i].size(); j++){
            std::vector<std::vector<cv::Point> > contTem = _contourHolder[i];
            std::vector<cv::Vec4i> temHierarchy = _hierarchyHolder[i];
            cv::Scalar color = cv::Scalar((0,255));
            cv::drawContours(tempDraw, contTem, j, color,2,8, temHierarchy,0, cv::Point());          
        }
        std::cout <<"Contours Drawn on Image: " << _contImages[i].getName() << std::endl;
        tempOut.getBitmap() = tempDraw;
        _drawnConts.pushBack(tempOut);
    }
}


ImageList<Image> ContourAssembler::getDrawnConts(){
    return _drawnConts;
}


void ContourAssembler::saveDrawnConts(){
    std::cout <<"Saving contour images:" <<std::endl;
    for(int i = 0; i < _drawnConts.size(); i++){
        _drawnConts[i].getFilePath() = _contImages[i].getName();
    }
    _drawnConts.save("/home/bgrassian/ROIProc Output/4_Drawn Contours");
    std::cout <<"Contour images saved." <<std::endl;
}


void ContourAssembler::saveDrawnContsInv(){
    _invertConts.save("/home/bgrassian/ROIProc Output/5_Drawn Contours Inv");
}

//Place in MorphOps Class
void ContourAssembler::invertContours(){
    for(int i=0; i < _drawnConts.size(); i++){
        cv::Mat tempIn = _drawnConts.getImage(i).getBitmap();
        cv::Mat tempOut;
        Image tempIm;
        cv::bitwise_not(tempIn, tempOut);
        tempIm.getBitmap()=tempOut;
        tempIm.getFilePath() = _drawnConts[i].getName();
        _invertConts.pushBack(tempIm);
    }
}

void ContourAssembler::morphDilate(){
    _drawnConts = _morphGetter.dilateProc(_drawnConts);


}
