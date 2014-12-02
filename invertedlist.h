/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _INVERTEDLIST_H_
#define _INVERTEDLIST_H_

#include <vector>
#include <set>

template<typename TS,typename TO>
class InvertedList {
public:

	InvertedList(TS * set):
	  featureCount(set->GetFeatureCount()) {
		  invertedList = new TO ** [featureCount];
		  invertedListLength = new int [featureCount];

		  for (int i=0; i < featureCount; ++i)
			  invertedListLength[i] = 0;

		  typename std::vector<TO *>:: const_iterator eit = set->begin();
		  typename std::vector<TO *>:: const_iterator eend = set->end();
		  while(eit!=eend) {
			  std::set<int>::const_iterator it = (*eit)->GetFeatureIterator();
			  std::set<int>::const_iterator end = (*eit)->GetFeatureIteratorEnd();
			  while(it!=end) {
				  ++(invertedListLength[*it]);
				  ++it;
			  }
			  ++eit;
		  }

		  for (int i=0; i < featureCount;++i)
			  invertedList[i] = new TO * [invertedListLength[i]];

		  for (int i=0; i < featureCount; ++i)
			  invertedListLength[i] = 0;

		  eit = set->begin();
		  while(eit!=eend) {
			  std::set<int>::const_iterator it = (*eit)->GetFeatureIterator();
			  std::set<int>::const_iterator end = (*eit)->GetFeatureIteratorEnd();
			  while(it!=end) {
				  invertedList[*it][invertedListLength[*it]++] = *eit;
				  ++it;
			  }
			  ++eit;
		  }
	  }

	  ~InvertedList() {
		  for(int i = 0;i<featureCount;++i)
			  delete invertedList[i];
		  delete invertedList;
		  delete invertedListLength;
	  }

	  inline int GetFeatureCount() { return featureCount; }

	  inline TO * Get(int feature, int pos) { 
		  return invertedList[feature][pos]; 
	  }

	  inline int GetLength(int feature) { 
		  return invertedListLength[feature]; 
	  }

private:
	int featureCount;
	TO *** invertedList;
	int * invertedListLength;
};

#endif // _INVERTEDLIST_H_
