/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include "prediction.h"

Prediction *  Prediction::Read(std::istream & in) {
	int id;
	in >> id;

	std::vector<std::pair<int,double> > evals;

	int category;
	while(in>>category) {
		double score;
		in >> score;
		evals.push_back(std::pair<int,double>(category,score));
	}

	return new Prediction(id,evals);
}

void  Prediction::Write(std::ostream & out) {
	out << id;
	std::vector<std::pair<int,double> >::const_iterator it = categories.begin();
	std::vector<std::pair<int,double> >::const_iterator end = categories.end();
	while(it!=end) {
		out << ' ' << it->first << ' ' << it->second;
		++it;
	}
	out << std::endl;
}
