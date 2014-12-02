/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _DISTRIBUTION_H_
#define _DISTRIBUTION_H_

#include <iostream>
#include <math.h>
#include "exampleset.h"

class Distribution {
public:

	enum InitializationType { UNDEFINED, UNIFORM, BALANCED };

	Distribution(int exampleCount,int categoryCount, InitializationType initializationType = UNDEFINED, ExampleSet * exampleSet = NULL):
	exampleCount(exampleCount),
		categoryCount(categoryCount) {
			distribution = new double * [exampleCount];
			for(int i = 0;i<exampleCount;++i)
				distribution[i] = new double[categoryCount];
			if(initializationType==UNIFORM)
				InitUniformDistribution();
			else if(initializationType==BALANCED)
				InitBalancedDistribution(exampleSet);
	}

	~Distribution() {
		for(int i = 0;i<exampleCount;++i)
			delete distribution[i];
		delete distribution;
	}


	inline double GetSum() {
		double sum = 0.0;
		for(int i = 0;i<exampleCount;++i)
			for(int j = 0;j<categoryCount;++j)
				sum += distribution[i][j];
		return sum;
	}

	void Normalize();

	void NormalizeByColumn();

	void NormalizeByRow();

	inline double GetValue(int example,int category) {
		return distribution[example][category];
	}

	inline void SetValue(int example,int category,double value) {
		distribution[example][category] = value;
	}

	inline void MulToValue(int example,int category,double value) {
		distribution[example][category] *= value;
	}

	inline int GetExampleCount() { return exampleCount; }


	inline int GetCategoryCount() { return categoryCount; }

	/**
	Data format:
	<number of examples>
	<number of categories>
	<probability value Example 0 Category 0> .. <probability value Example 0 Category n>
	..
	<probability value Example m Category 0> .. <probability value Example m Category n>
	*/
	static Distribution * Read(std::istream & in);

	/** Writes the object to the \a std::osteam \a out.
	@param[in] out stream to write to
	*/
	void Write(std::ostream & out);

private:

	void InitUniformDistribution();
	void InitBalancedDistribution(ExampleSet * exampleSet);

	int exampleCount;
	int categoryCount;
	double ** distribution;
};

#endif // _DISTRIBUTION_H_
