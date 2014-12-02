/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _EXAMPLESET_H_
#define _EXAMPLESET_H_

#include <set>
#include <vector>
#include <istream>
#include "example.h"

class ExampleSet : public std::vector<Example *> {
 public:

  ExampleSet():categoryCount(0),featureCount(0) {}

  ExampleSet(int categoryCount, int featureCount):
    categoryCount(categoryCount),featureCount(featureCount) {}

  ~ExampleSet() {
    std::vector<Example *>::const_iterator it = begin();
    std::vector<Example *>::const_iterator tend = end();
    while(it!=tend) {
      delete *it;
      ++it;
    }
  }

  inline int GetCategoryCount() { 
	  return categoryCount; 
  }

  inline int GetFeatureCount() { 
	  return featureCount; 
  }

  void ResetIds() {
    std::vector<Example *>::const_iterator it = begin();
    std::vector<Example *>::const_iterator tend = end();
    int id = 0;
    while(it!=tend) {
      (*it)->SetId(id);
      ++id;
      ++it;
    }
  }

  /** Data format, one example per line:
      <Example 0>
	  ..
      <Example n>

  */
  static ExampleSet * Read(std::istream & in,std::set<int> * features=NULL);
  
  void Write(std::ostream & out);
  
 private:
  int categoryCount;
  int featureCount;
};

#endif // _EXAMPLESET_H_
