#include "DataFormats/Common/interface/Wrapper.h"

#include <vector>

namespace CSCOfflineTiming_CSCTimingBabyMaker
{
    struct dictionary
    {
        std::vector<std::vector<double> > dummy01;
        std::vector<std::vector<char> > dummy02;

        std::vector<std::vector<std::vector<unsigned int> > > dummy03;
        std::vector<std::vector<std::vector<int> > > dummy04;
        std::vector<std::vector<std::vector<bool> > > dummy05;

        std::vector<std::vector<std::vector<std::vector<int> > > > dummy06;
        std::vector<std::vector<std::vector<std::vector<char> > > > dummy07;
        std::vector<std::vector<std::vector<std::vector<double> > > > dummy08;


        std::vector<std::vector<int> >::iterator it00;
        std::vector<std::vector<double> >::iterator it01;
        std::vector<std::vector<char> >::iterator it02;

        std::vector<std::vector<std::vector<unsigned int> > >::iterator it03;
        std::vector<std::vector<std::vector<int> > >::iterator it04;
        std::vector<std::vector<std::vector<bool> > >::iterator it05;

        std::vector<std::vector<std::vector<std::vector<int> > > >::iterator it06;
        std::vector<std::vector<std::vector<std::vector<char> > > >::iterator it07;
        std::vector<std::vector<std::vector<std::vector<double> > > >::iterator it08;


        edm::Wrapper<std::vector<std::vector<double> > > dummy1;
        edm::Wrapper<std::vector<std::vector<char> > > dummy2;

        edm::Wrapper<std::vector<std::vector<std::vector<unsigned int> > > > dummy3;
        edm::Wrapper<std::vector<std::vector<std::vector<int> > > > dummy4;
        edm::Wrapper<std::vector<std::vector<std::vector<bool> > > > dummy5;

        edm::Wrapper<std::vector<std::vector<std::vector<std::vector<int> > > > > dummy6;
        edm::Wrapper<std::vector<std::vector<std::vector<std::vector<char> > > > > dummy7;
        edm::Wrapper<std::vector<std::vector<std::vector<std::vector<double> > > > > dummy8;
    };
}
