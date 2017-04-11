#include "morphops.h"
#include <lightfieldcorrector.h>
#include <iostream>


MorphOps::MorphOps(){
}

void MorphOps::convertProc(ImageList<Image> inConv){
    std::cout << "Converting " << inConv.size() <<  " images:" << std::endl;
    for(int i = 0; i < inConv.size(); i++){
        inConv.getImage(i).to8Bit();
        inConv.getImage(i).toGrey();
        Image tempI;
        cv::Mat tempInM;
        tempInM=inConv.getImage(i).getBitmap();
//        tempInM.convertTo(tempInM, CV_8U, 0.00390625);
        tempI.getBitmap()= tempInM;
        tempI.getFilePath()=inConv.getImage(i).getName();
        _imListC.pushBack(tempI);
        std::cout <<"Converted image: " << i << std::endl;
    };
}

/*ImageList<Image>*/void MorphOps::threshProc(){
    for(int j=0; j< _imListC.size(); j++){
        cv::Mat tempIn;
        cv::Mat tempOut;
        cv::Mat tempFinal;
        Image tempIm;
        tempIn = _imListC.getImage(j).get8Bitmap();
        cv::adaptiveThreshold(tempIn, tempOut, 255,  CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,71,1);
        cv::morphologyEx(tempOut, tempFinal, CV_MOP_OPEN, cv::Mat(), cv::Point (-1,-1), 2, 1 ,1);
        tempIm.getBitmap()=tempFinal;
        tempIm.getFilePath()=_imListC.getImage(j).getName();
        _imListThr.pushBack(tempIm);
        std::cout << "Thresholded Image: " << j << std::endl;
    }
//    return(_imListThr);
}


void MorphOps::filterProc(){
    ImageList<Image> fVec;
    for(int i=0; i <_imListC.size(); i++){
        cv::Mat in = _imListC.getImage(i).getBitmap();
        cv::Mat out;
        cv::adaptiveBilateralFilter(in, out, cv::Size(15,15), 20, 20);
        Image outIm;
        outIm.getBitmap()= out;
        outIm.getFilePath()=_imListC.getImage(i).getName();
        fVec.pushBack(outIm);
//        _imListC.getImage(i)=outIm;
        std::cout <<"Bilateral Filter img: " <<i<<std::endl;
    }
    std::cout <<"saving"<<std::endl;
    fVec.save("/home/bgrassian/filter");
    std::cout <<"saved"<<std::endl;
}

void MorphOps::SaveThresh(){
    for(int i=0; i<_imListThr.size(); i++){ // add try
        _imListThr[i].getFilePath() = _imListC[i].getName();
    }
    std::cout <<"Saving threshed images" << std::endl;
    _imListThr.save("/home/bgrassian/ROIProc Output/3_Thresh Ims");
    std::cout <<"Output saved" << std::endl;
}

ImageList<Image> MorphOps::dilateProc(ImageList<Image> &inIms){
    ImageList<Image> dilatedIms;
    for(int i=0; i< inIms.size(); i++){
        cv::Mat tempIn = inIms.getImage(i).getBitmap();
        Image tempImg;
        cv::Mat tempOut;
        cv::dilate(tempIn, tempOut, cv::Mat(), cv::Point(-1, 1), 1 ,1 ,1);
        tempImg.getBitmap()=tempOut;
        dilatedIms.pushBack(tempImg);
    }
    return dilatedIms;
}

ImageList<Image> MorphOps::erodeProc(ImageList<Image> &inImL){
    ImageList<Image> erodeIms;
    for(int i=0; i <  inImL.size(); i++){
        cv::Mat tempIn = inImL.getImage(i).getBitmap();
        cv::Mat tempOut;
        cv::erode(tempIn, tempOut, cv::Mat(), cv::Point(-1),1,1,1);
        Image out;
        out.getBitmap() = tempOut;
        erodeIms.pushBack(out);
    }
    return erodeIms;
}

void MorphOps::setConvList(ImageList<Image> conv){
    _imListC = conv;
}

ImageList<Image> MorphOps::getConvList(){
    return(_imListC);
}

void MorphOps::setThreshList(ImageList<Image> inThresh){
    _imListThr = inThresh;
}

ImageList<Image> MorphOps::getThreshList(){
    return(_imListThr);
}


