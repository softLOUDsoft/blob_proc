#ifndef CONTOURASSEMBLER_H
#define CONTOURASSEMBLER_H
#include "morphops.h"
#include <iostream>


class ContourAssembler
{
public:
    ContourAssembler();
    void runMorphOps(ImageList<Image> &startList);
    void findEdges(/*ImageList<Image> inList*/);
    void findConts();
    void drawContours();
    void saveDrawnConts();
    ImageList<Image> getDrawnConts();
    void invertContours();
    void saveDrawnContsInv();
    void morphDilate();
    ImageList<Image> getInList();
    void setImages(ImageList<Image> in);
    void relayGetConv();


private:
    ImageList<Image> _invertConts;
    MorphOps _morphGetter;
    ImageList<Image> _contImages;    
    std::vector<std::vector<std::vector<cv::Point> > > _contourHolder;
    std::vector<std::vector<cv::Vec4i> > _hierarchyHolder;
    ImageList<Image> _drawnConts;

};

#endif // CONTOURASSEMBLER_H
