/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _DOCUMENTSET_H_
#define _DOCUMENTSET_H_

#include <vector>
#include <istream>
#include "document.h"


class DocumentSet : public std::vector<Document *>  {
 public:

  DocumentSet():featureCount(0) {}


  DocumentSet(int featureCount):featureCount(featureCount) 
  {}

  ~DocumentSet() {
    std::vector<Document *>::const_iterator it = begin();
    std::vector<Document *>::const_iterator tend = end();
    while(it!=tend) {
      delete *it;
      ++it;
    }
  }

  inline int GetFeatureCount() { return featureCount; }
  
  /** Data format, one document per line:
      <Document 0>
	  ..
      <Document n>
  */
  static DocumentSet * Read(std::istream & in); 
  
  void Write(std::ostream & out);
  
 private:
  int featureCount;
};

#endif // _DOCUMENTSET_H_
