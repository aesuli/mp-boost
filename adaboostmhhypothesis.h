/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _ADABOOSTMHHYPOTHESIS_H_
#define _ADABOOSTMHHYPOTHESIS_H_

#include <vector>
#include <istream>

class AdaBoostMHHypothesis {
 public:

 AdaBoostMHHypothesis(int categoryCount):
  categoryCount(categoryCount) {
    feature = 0;
    score = 0;
    c0 = new double[categoryCount];
    c1 = new double[categoryCount];
  }
  
  ~AdaBoostMHHypothesis() {
    delete c0;
    delete c1;
  }
  
  inline int GetFeature() { return feature; }

  inline void SetFeature(int t) { feature = t; }
  
  inline double GetScore() { return score; }

  inline void SetScore(double s) { score = s; }
  
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
      <feature>
      <score>
      <c0 0> .. <c0 n>
      <c1 0> .. <c1 n>
  */
  static AdaBoostMHHypothesis * Read(std::istream & in);

  void Write(std::ostream &out);

 private:  
  int feature;
  double score;
  int categoryCount;
  double* c0;
  double* c1;
};

#endif // _ADABOOSTMHHYPOTHESIS_H_
