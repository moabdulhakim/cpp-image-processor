#pragma once
#include "Filter.h"

#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

class Brightness : public Filter
{
    double value;

public:
    Brightness(Image &img);
    static string getName();
    static string getId();
    void apply() override;
    void getNeeds() override;
};

#endif // BRIGHTNESS_H
