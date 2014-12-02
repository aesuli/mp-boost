/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include "documentset.h"
#include <string>
#include <sstream>

DocumentSet * DocumentSet::Read(std::istream & in) {
  DocumentSet * documentSet = new DocumentSet();

  std::string line;
  while(std::getline(in,line)) {
	std::istringstream buffer(line);
	Document * doc = Document::Read(buffer);

	int count = doc->GetFeatureCount();
	if(count>0) {
	  int feature = *(--doc->GetFeatureIteratorEnd());
	  if(documentSet->featureCount<=feature)
	    documentSet->featureCount = feature+1;
	}

	documentSet->push_back(doc);
  }

  return documentSet;
}
  
void DocumentSet::Write(std::ostream & out) {
  std::vector<Document *>::const_iterator it = begin();
  std::vector<Document *>::const_iterator tend = end();
  while(it!=tend) {
    (*it)->Write(out);
    ++it;
  }
}
