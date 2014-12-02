/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include "predictionset.h"
#include <string>
#include <sstream>

PredictionSet * PredictionSet::Read(std::istream & in) {
  PredictionSet * predictionSet = new PredictionSet();

  std::string line;
  while(std::getline(in,line)) {
	std::istringstream buffer(line);
	Prediction * prediction = Prediction::Read(buffer);

	int count = prediction->GetCategoryCount();
	if(count>0) {
	  std::pair<int,double> eval = *(--prediction->GetCategoryIteratorEnd());
	  if(predictionSet->categoryCount<=eval.first)
	    predictionSet->categoryCount = eval.first+1;
	}
	
	predictionSet->push_back(prediction);
  }

  return predictionSet;
}
  
void PredictionSet::Write(std::ostream & out) {
  std::vector<Prediction *>::const_iterator it = begin();
  std::vector<Prediction *>::const_iterator tend = end();
  while(it!=tend) {
    (*it)->Write(out);
    ++it;
  }
}
