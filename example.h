/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include <istream>
#include <vector>
#include <set>
#include <algorithm>
#include "document.h"

class Example : public Document
{
 public:

  Example(int id,const std::vector<int> & features,
	  const std::vector<int> & categories):
    Document(id,features),categorySet(categories.begin(),categories.end()) {
  }
  
  inline void SetId(int newId) { 
	  id = newId; 
  }

  inline int GetCategoryCount() { 
	  return categorySet.size(); 
  }

  inline bool HasCategory(int category) {
    return categorySet.find(category)!=categorySet.end();
  }
  
  inline std::set<int>::const_iterator GetCategoryIterator() { 
    return categorySet.begin(); 
  }

  inline std::set<int>::const_iterator GetCategoryIteratorEnd() { 
    return categorySet.end(); 
  }

  /** Data format:
      <document id> [<feature id>+] [: <category id>+]
  */
  static Example * Read(std::istream & in, std::set<int> * selectedFeatures = NULL);

  void Write(std::ostream & out);

 private:
  std::set<int> categorySet;
};

#endif // _EXAMPLE_H_
