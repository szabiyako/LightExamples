#pragma once

class ProgressNotifier
{
public:
    ProgressNotifier();
    virtual ~ProgressNotifier();
    virtual void start(const int startValue, const int endValue) = 0;
    virtual void setProgress(const int value) = 0;
    virtual void finish() = 0;
};

