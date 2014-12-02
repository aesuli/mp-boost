/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _MPBOOSTHYPOTHESIS_H_
#define _MPBOOSTHYPOTHESIS_H_

#include <vector>
#include <istream>

class MPBoostHypothesis {
 public:

  MPBoostHypothesis(int categoryCount):
    categoryCount(categoryCount) {
    features = new int[categoryCount];
    scores = new double[categoryCount];
    c0 = new double[categoryCount];
    c1 = new double[categoryCount];
  }
  
  ~MPBoostHypothesis() {
    delete features;
    delete scores;
    delete c0;
    delete c1;
  }
  
  inline int GetFeature(int pos) { return features[pos]; }

  inline void SetFeature(int pos,int t) { features[pos] = t; }
  
  inline double GetScore(int pos) { return scores[pos]; }

  inline void SetScore(int pos,double s) { scores[pos] = s; }
  
  inline int GetCategoryCount() { return categoryCount; }
  
  inline double * GetC0() { return c0; }

  inline double GetC0(int pos) { return c0[pos]; }

  inline void SetC0(int pos,double val) { c0[pos] = val; }

  inline void AddToC0(int pos,double val) { c0[pos] += val; }

  inline void MulToC0(int pos,double val) { c0[pos] *= val; }

  inline double * GetC1() { return c1; }

  inline double GetC1(int pos) { return c1[pos]; }

  inline void SetC1(int pos,double val) { c1[pos] = val; }

  inline void AddToC1(int pos,double val) { c1[pos] += val; }

  inline void MulToC1(int pos,double val) { c1[pos] *= val; }

  /** Data format:
      <number of categories>
      <feature 0> .. <feature n>
      <score 0> .. <score n>
      <c0 0> .. <c0 n>
      <c1 0> .. <c1 n>
  */
  static MPBoostHypothesis * Read(std::istream & in);

  void Write(std::ostream &out);

 private:  
  int* features;
  double* scores;
  int categoryCount;
  double* c0;
  double* c1;
};

#endif // _MPBOOSTHYPOTHESIS_H_
