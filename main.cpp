#include <iostream>
#include <lightfieldcorrector.h>
#include "morphops.h"
#include "blobdetection.h"
#include "contourassembler.h"
#include "bloblibproc.h"


int main()
{

//    BlobLibProc procCrops;
//    procCrops.loadBlobDir();
//    procCrops.cullBlobs();

    boost::filesystem::path pa("/home/bgrassian/filter");
    ImageList<Image> inList;
    boost::filesystem::directory_iterator end_itr;
    int numbIm=0;
    for (boost::filesystem::directory_iterator itr(pa); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path())) {
            std::string current_file = itr->path().string();
            std::string ext =itr-> path().extension().string();
            boost::algorithm::to_lower(ext);
            Image temp;
            cv::Mat tempa = cv::imread(current_file, CV_8U);
            temp.getFilePath()=current_file;
            temp.getBitmap() = tempa;
            inList.pushBack(temp);
            numbIm++;
            std::cout<<"Loaded image #: " <<numbIm <<std::endl;
        }
    }


//    MorphOps filter;
//    filter.convertProc(inList);
//    filter.filterProc();

    BlobDetection blobImages;
    blobImages.runMorphs(inList);
    blobImages.runDetect();
    blobImages.setBlobUnc(inList);
//    blobImages.load16BitRaw();
    blobImages.extractBlobs();
    blobImages.saveExtractBlobs();
    blobImages.statBlobs();


    std::cout <<"All operations completed." << std::endl;

    return 0;
}




//    boost::shared_ptr<ImageList<Image> >  inList(new ImageList<Image>);
//    boost::filesystem::path p("/home/bgrassian/subset");
//    inList->addImagesFromDir(pa);


