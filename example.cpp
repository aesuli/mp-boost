/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include "example.h"
#include "defs.h"
#include <string>
#include <sstream>

Example * Example::Read(std::istream & in, std::set<int> * selectedFeatures) {
  int id;
  in >> id;

  std::string token;

  std::vector<int> features;
  std::vector<int> categories;

  bool readingCategories = false;
  if(selectedFeatures==NULL) {
    while(in >> token) {
      if(token==CAT_SEPARATOR)
	readingCategories = true;
      else {
	std::istringstream stream(token);
	int value;
	stream >> value;
	if(readingCategories)
	  categories.push_back(value);
	else
	  features.push_back(value);
      }
    }
  }
  else {
    std::set<int>::const_iterator sle = selectedFeatures->end();
    while(in >> token) {
      if(token==CAT_SEPARATOR)
	readingCategories = true;
      else {
	std::istringstream stream(token);
	int value;
	stream >> value;
	if(readingCategories)
	  categories.push_back(value);
	else {
	  if(selectedFeatures->find(value)!=sle)
	    features.push_back(value);
	}
      }
    }
  }
  return new Example(id,features,categories);
}

void Example::Write(std::ostream & out) {
  out << id ;

  std::set<int>::const_iterator it = featureSet.begin();
  std::set<int>::const_iterator end = featureSet.end();
  while(it!=end) {
    out << ' ' << *it;
    ++it;
  }

  out << " ";
  out << CAT_SEPARATOR;

  it = categorySet.begin();
  end = categorySet.end();
  while(it!=end) {
    out << ' ' << *it;
    ++it;
  }
  out << std::endl;
}
