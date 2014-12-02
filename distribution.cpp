/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include "distribution.h"
#include "example.h"
#include <vector>
#include <set>

void Distribution::InitUniformDistribution() {
  double value = 1.0/(double)(exampleCount*categoryCount);
  for(int i = 0;i<exampleCount;++i)
    for(int j = 0;j<categoryCount;++j)
      distribution[i][j] = value;
}

void Distribution::InitBalancedDistribution(ExampleSet * exampleSet) {
  int * posCount = new int [categoryCount];
  for(int i = 0;i<categoryCount;++i)
    posCount[i] = 0;

  std::vector<Example *>::const_iterator eit = exampleSet->begin();
  std::vector<Example *>::const_iterator eend = exampleSet->end();
  while(eit!=eend) {
    std::set<int>::const_iterator iit = (*eit)->GetCategoryIterator();
    std::set<int>::const_iterator iend = (*eit)->GetCategoryIteratorEnd();
    while(iit!=iend) {
      ++posCount[*iit];
      ++iit;
    }
    ++eit;
  }

  double * posWeight = new double[categoryCount];
  double * negWeight = new double[categoryCount];

  double halfPerCategory = 0.5/categoryCount;
  for(int i = 0;i<categoryCount;++i) {
    posWeight[i] = halfPerCategory/posCount[i];
    negWeight[i] = halfPerCategory/(exampleSet->size()-posCount[i]);
  }

  eit = exampleSet->begin();
  while(eit!=eend) {
    int cat = 0;
    Example * example = *eit;
    std::set<int>::const_iterator iit = example->GetCategoryIterator();
    std::set<int>::const_iterator iend = example->GetCategoryIteratorEnd();
    while(iit!=iend) {
      while(cat!=*iit) {
	SetValue(example->GetId(),cat,negWeight[cat]);
	++cat;
      }
      SetValue(example->GetId(),*iit,posWeight[cat]);
      ++iit;
    }
    while(cat!=categoryCount) {
      SetValue(example->GetId(),cat,negWeight[cat]);
      ++cat;
    }
    ++eit;
  }

  delete posCount;
  delete posWeight;
  delete negWeight;
}

void Distribution::Normalize() {
  double sum = GetSum();    
  if(sum!=1.0) {
    for(int i = 0;i<exampleCount;++i)
      for(int j = 0;j<categoryCount;++j)
	distribution[i][j] /= sum;
  }
}

void Distribution::NormalizeByColumn() {
  double * sums = new double[categoryCount];
  for(int j = 0;j<categoryCount;++j)
    sums[j] = 0.0;
  
  for(int i = 0;i<exampleCount;++i) {
    for(int j = 0;j<categoryCount;++j)
      sums[j] += distribution[i][j];
  }
  
  for(int j = 0;j<categoryCount;++j)
    sums[j] *= categoryCount;
	
  for(int i = 0;i<exampleCount;++i) {
    for(int j = 0;j<categoryCount;++j)
      distribution[i][j] /= sums[j];
  }
  delete sums;
}

void Distribution::NormalizeByRow() {
  for(int i = 0;i<exampleCount;++i) {
    double sum = 0.0;    
    for(int j = 0;j<categoryCount;++j) 
      sum += distribution[i][j];
    sum *= exampleCount;
    if(sum!=1.0) {
      for(int j = 0;j<categoryCount;++j) 
	distribution[i][j] /= sum;
    }
  }
}

Distribution * Distribution::Read(std::istream & in) {
  int exampleCount;
  int categoryCount;
  in >> exampleCount;
  in >> categoryCount;
  Distribution * distribution = new Distribution(exampleCount,categoryCount);
  for(int i = 0;i<exampleCount;++i)
    for(int j = 0;j<categoryCount;++j)
      in >> distribution->distribution[i][j];
  return distribution;
}

void Distribution::Write(std::ostream & out) {
  out << exampleCount << std::endl;
  out << categoryCount << std::endl;
  for(int i = 0;i<exampleCount;++i) {
    for(int j = 0;j<categoryCount;++j) {
      double val = distribution[i][j];
      if(isnan(val))
	out << "0 ";
      else
	out << val << ' ';
    }
    out << std::endl;
  }
}
