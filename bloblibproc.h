#ifndef BLOBLIBPROC_H
#define BLOBLIBPROC_H

#include <string>
#include <lightfieldcorrector.h>
#include <iostream>


class BlobLibProc
{
public:
    BlobLibProc();
    void printTest();
    void cullBlobs();
    void saveSurvive();
    void loadBlobDir();


private:
    ImageList<Image> _blobDir;
    ImageList<Image> _blobSurvive;

};




#endif // BLOBLIBPROC_H
