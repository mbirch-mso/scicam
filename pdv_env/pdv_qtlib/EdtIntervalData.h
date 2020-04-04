#ifndef EDT_INTERVAL_DATA_H_
#define EDT_INTERVAL_DATA_H_

class EdtInterval
{
    double low;
    double high;
public:

    EdtInterval();
    EdtInterval(double l, double h);
    
    double Low() const;
    double High() const;

    void Set(double l, double h);
    void SetLow(double l);
    void SetHigh(double h);

};

class EdtIntervalData
{
    double value;
    EdtInterval interval;

public:
    EdtIntervalData();
    EdtIntervalData(double v, double l, double h);
    EdtIntervalData(double v, EdtInterval &intv);
    virtual ~EdtIntervalData(void);

    double Value();
    void SetValue(double v);

    const EdtInterval &Interval();
    void SetInterval(const EdtInterval &intv);

};

#endif
