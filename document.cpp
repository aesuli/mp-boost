/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include "document.h"

Document * Document::Read(std::istream & in) {
  int id;
  in >> id;

  std::vector<int> features;

  int feature;
  while(in>>feature)
	  features.push_back(feature);

  return new Document(id,features);
}

void Document::Write(std::ostream & out) {
  out << id ;

  std::set<int>::const_iterator it = featureSet.begin();
  std::set<int>::const_iterator end = featureSet.end();
  while(it!=end) {
    out << ' ' << *it;
    ++it;
  }
  out << std::endl;
}
