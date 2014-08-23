//
//  sector.h
//  YorkshireTea
//
//  Created by Karl Jacques on 29/05/2014.
//
//

#ifndef __YorkshireTea__sector__
#define __YorkshireTea__sector__

#include <iostream>

struct Vertex
{
    float x;
    float y;
    
    float height;
    
};

class Sector
{
public:
    
    Sector();
    
    Sector*    t1;
    Sector*    t2;
    Sector*    t3;
    Sector*    t4;
    
    Vertex      v1;
    Vertex      v2;
    Vertex      v3;
    Vertex      v4;
    
};

#endif /* defined(__YorkshireTea__sector__) */
