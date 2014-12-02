/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _PREDICTION_H_
#define _PREDICTION_H_

#include <istream>
#include <vector>
#include <algorithm>

class Prediction
{
public:

	Prediction(int id,const std::vector<std::pair<int,double> > & categories):id(id),categories(categories) {
		sort(this->categories.begin(),this->categories.end());
	}

	inline int GetId() { return id; }


	inline int GetCategoryCount() { return categories.size(); }

	inline std::vector<std::pair<int,double> >::const_iterator GetCategoryIterator() { 
		return categories.begin(); 
	}

	inline std::vector<std::pair<int,double> >::const_iterator GetCategoryIteratorEnd() { 
		return categories.end(); 
	}

	/** Data format:
	<document id> [<category id > <score>]
	*/
	static Prediction * Read(std::istream & in);

	void Write(std::ostream & out);

private:
	int id;
	std::vector<std::pair<int,double> > categories;
};

#endif // _PREDICTION_H_
