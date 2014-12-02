/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _PREDICTIONSET_H_
#define _PREDICTIONSET_H_

#include <vector>
#include <istream>
#include "prediction.h"

class PredictionSet: public std::vector<Prediction *> {
 public:

  PredictionSet():categoryCount(0) {}
  
  PredictionSet(int categoryCount):
    categoryCount(categoryCount) {}
  
  ~PredictionSet() {
    std::vector<Prediction *>::const_iterator it = begin();
    std::vector<Prediction *>::const_iterator tend = end();
    while(it!=tend) {
      delete *it;
      ++it;
    }
  }
  
  inline int GetCategoryCount() { return categoryCount; }
  
  /** Data format, one prediction per line:
      <Evaluation 0>
	  ..
      <Evaluation n>
  */
  static PredictionSet * Read(std::istream & in);
  
  void Write(std::ostream & out);
  
 private:
  int categoryCount;
};

#endif // _PREDICTIONSET_H_
