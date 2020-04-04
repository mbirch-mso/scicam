#include "EdtIntervalData.h"


EdtInterval::EdtInterval()
{
    low = 0;
    high = 1;
}

EdtInterval::EdtInterval(double l, double h)

{
    low = l;
    high = h;
}

double EdtInterval::Low() const
{
    return low;
}

double EdtInterval::High() const
{
    return high;
}


void EdtInterval::Set(double l, double h)

{
    low = l;
    high = h;
}

void EdtInterval::SetLow(double l)

{
    low = l;
}

void EdtInterval::SetHigh(double h)
{
    high = h;
}


EdtIntervalData::EdtIntervalData(void)
{
}

EdtIntervalData::~EdtIntervalData(void)
{
}
EdtIntervalData::EdtIntervalData(double v, double l, double h) :
interval(l,h)

{
    value = v;
}

EdtIntervalData::EdtIntervalData(double v, EdtInterval &intv)

{
    value = v;
    interval = intv;
}

double EdtIntervalData::Value()
{
    return value;
}

void EdtIntervalData::SetValue(double v)
{
    value = v;
}


const EdtInterval & EdtIntervalData::Interval()
{
    return interval;
}

void EdtIntervalData::SetInterval(const EdtInterval &intv)
{
    interval = intv;
}
