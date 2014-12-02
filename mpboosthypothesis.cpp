/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include <math.h>
#include "mpboosthypothesis.h"

MPBoostHypothesis * MPBoostHypothesis::Read(std::istream & in) {
  int categoryCount;
  in >> categoryCount;
  MPBoostHypothesis * hyp = new MPBoostHypothesis(categoryCount);
  for(int i = 0;i<categoryCount;++i)
    in >> hyp->features[i];
  for(int i = 0;i<categoryCount;++i)
    in >> hyp->scores[i];
  for(int i = 0;i<categoryCount;++i)
    in >> hyp->c0[i];
  for(int i = 0;i<categoryCount;++i)
    in >> hyp->c1[i];
  return hyp;
}

void MPBoostHypothesis::Write(std::ostream &out) {
  out << categoryCount << std::endl;
  for(int i = 0;i<categoryCount;++i)
    out << features[i] << ' ';
  out << std::endl;
  for(int i = 0;i<categoryCount;++i) {
    double val = scores[i];
    if(isnan(val))
      out << "0 ";
    else
      out << val << ' ';
  }
  out << std::endl;
  for(int i = 0;i<categoryCount;++i) {
    double val = c0[i];
    if(isnan(val))
      out << "0 ";
    else
      out << val << ' ';
  }
  out << std::endl;
  for(int i = 0;i<categoryCount;++i) {
    double val = c1[i];
    if(isnan(val))
      out << "0 ";
    else
      out << val << ' ';
  }
  out << std::endl;
}
