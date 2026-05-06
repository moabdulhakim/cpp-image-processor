#pragma once
#include <string>
#include "Filter.h"

#ifndef WHITEANDBLACK_H
#define WHITEANDBLACK_H

class WhiteAndBlack : public Filter
{
public:
    WhiteAndBlack(Image &img);
    static string getName();
    static string getId();
    void apply() override;
    void getNeeds() override;
};

#endif // WHITEANDBLACK_H
