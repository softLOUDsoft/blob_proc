#ifndef MORPHOPS_H
#define MORPHOPS_H
#include <string>
#include <lightfieldcorrector.h>
#include <iostream>



class MorphOps
{
public:
    MorphOps();
    void convertProc(ImageList<Image> inConv);
    /*ImageList<Image>*/void threshProc();
    void SaveThresh();
    void setConvList(ImageList<Image> conv);
    ImageList<Image> getConvList();
    void setThreshList(ImageList<Image> inThresh);
    ImageList<Image> getThreshList();
    ImageList<Image> dilateProc(ImageList<Image> &inIms);
    ImageList<Image> erodeProc(ImageList<Image> &inImL);
    void filterProc();




private:
    ImageList<Image> _imListC;
    ImageList<Image> _imListThr;
};

#endif // MORPHOPS_H
