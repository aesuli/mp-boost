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
#include "adaboostmhhypothesis.h"

AdaBoostMHHypothesis * AdaBoostMHHypothesis::Read(std::istream & in) {
  int categoryCount;
  in >> categoryCount;
  AdaBoostMHHypothesis * hyp = new AdaBoostMHHypothesis(categoryCount);
  in >> hyp->feature;
  in >> hyp->score;
  for(int i = 0;i<categoryCount;++i)
    in >> hyp->c0[i];
  for(int i = 0;i<categoryCount;++i)
    in >> hyp->c1[i];
  return hyp;
}

void AdaBoostMHHypothesis::Write(std::ostream &out) {
  out << categoryCount << std::endl;
  out << feature << std::endl;
  if(isnan(score))
    out << "0 ";
  else
    out << score;
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
