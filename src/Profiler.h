#ifndef PROFILER_H
#define PROFILER_H

#include "common.h"


class Profiler
{
    public:

        struct sample
        {
            int avg_time;               //the average time in ms
            long spp_count;              //the number of samples received

            sample(int, int);
        };

        Profiler(string);
        virtual ~Profiler();

        void start();
        void stop();

    protected:

    private:

        string name;
        int start_time;

        static map<string, sample*> samples;
};

#endif // PROFILER_H
