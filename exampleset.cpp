/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include "exampleset.h"
#include <string>
#include <sstream>

ExampleSet * ExampleSet::Read(std::istream & in,std::set<int> * features) {
  ExampleSet * exampleSet = new ExampleSet();

  std::string line;
  while(std::getline(in,line)) {
	std::istringstream buffer(line);
	Example * example = Example::Read(buffer,features);

	int count = example->GetFeatureCount();
	if(count>0) {
	  int feature = *(--example->GetFeatureIteratorEnd());
	  if(exampleSet->featureCount<=feature)
	    exampleSet->featureCount = feature+1;
	}
	
	count = example->GetCategoryCount();
	if(count>0) {
	  int category = *(--example->GetCategoryIteratorEnd());
	  if(exampleSet->categoryCount<=category)
	    exampleSet->categoryCount = category+1;
	}

	exampleSet->push_back(example);
  }

  return exampleSet;
}
  
void ExampleSet::Write(std::ostream & out) {
  std::vector<Example *>::const_iterator it = begin();
  std::vector<Example *>::const_iterator tend = end();
  while(it!=tend) {
    (*it)->Write(out);
    ++it;
  }
}
