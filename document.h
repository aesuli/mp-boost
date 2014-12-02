/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include <istream>
#include <vector>
#include <set>
#include <algorithm>

class Document
{
public:

	Document(int id,const std::vector<int> & features):id(id),
		featureSet(features.begin(),features.end())
	{
	}

	inline int GetId() { return id; }

	inline int GetFeatureCount() { return featureSet.size(); }

	inline bool HasFeature(int feature) {
		return featureSet.find(feature)!=featureSet.end();
	}

	inline std::set<int>::const_iterator GetFeatureIterator() { 
		return featureSet.begin(); 
	}

	inline std::set<int>::const_iterator GetFeatureIteratorEnd() { 
		return featureSet.end(); 
	}

	/** Data format:
	<example id> [<feature id>+]
	*/
	static Document * Read(std::istream & in);

	void Write(std::ostream & out);

protected:
	int id;
	std::set<int> featureSet;
};

#endif // _DOCUMENT_H_
