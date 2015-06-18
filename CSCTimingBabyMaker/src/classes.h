#include "DataFormats/Common/interface/Wrapper.h"

#include <vector>

namespace CSCOfflineTiming_CSCTimingBabyMaker
{
    struct dictionary
    {
        std::vector<std::vector<double> > dummy01;        

        std::vector<std::vector<std::vector<unsigned int> > > dummy02;
        std::vector<std::vector<std::vector<int> > > dummy03;
        std::vector<std::vector<std::vector<bool> > > dummy04;

        std::vector<std::vector<std::vector<std::vector<int> > > > dummy05;
        std::vector<std::vector<std::vector<std::vector<char> > > > dummy06;
        std::vector<std::vector<std::vector<std::vector<double> > > > dummy07;


        std::vector<std::vector<int> >::iterator it00;
        std::vector<std::vector<double> >::iterator it01;

        std::vector<std::vector<std::vector<unsigned int> > >::iterator it02;
        std::vector<std::vector<std::vector<int> > >::iterator it03;
        std::vector<std::vector<std::vector<bool> > >::iterator it04;

        std::vector<std::vector<std::vector<std::vector<int> > > >::iterator it05;
        std::vector<std::vector<std::vector<std::vector<char> > > >::iterator it06;
        std::vector<std::vector<std::vector<std::vector<double> > > >::iterator it07;


        edm::Wrapper<std::vector<std::vector<double> > > dummy1;        

        edm::Wrapper<std::vector<std::vector<std::vector<unsigned int> > > > dummy2;
        edm::Wrapper<std::vector<std::vector<std::vector<int> > > > dummy3;
        edm::Wrapper<std::vector<std::vector<std::vector<bool> > > > dummy4;

        edm::Wrapper<std::vector<std::vector<std::vector<std::vector<int> > > > > dummy5;
        edm::Wrapper<std::vector<std::vector<std::vector<std::vector<char> > > > > dummy6;
        edm::Wrapper<std::vector<std::vector<std::vector<std::vector<double> > > > > dummy7;
    };
}
