/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _EVALUATION_H_
#define _EVALUATION_H_

#include <istream>
#include "defs.h"

class Evaluation {
  public:

  Evaluation(int categoryCount);

  ~Evaluation();

  void Clear();

  void Merge(Evaluation * evaluation);

  inline int GetCategoryCount() { return categoryCount; }

  inline void SetTP(int value) {
    globalTable[RES_TRUE][RES_POSITIVE] = value;
  }

  inline void SetFP(int value) {
    globalTable[RES_FALSE][RES_POSITIVE] = value;
  }

  inline void SetTN(int value) {
    globalTable[RES_TRUE][RES_NEGATIVE] = value;
  }

  inline void SetFN(int value) {
    globalTable[RES_FALSE][RES_NEGATIVE] = value;
  }

  inline void SetTP(int category,int value) {
    categoryTable[category][RES_TRUE][RES_POSITIVE] = value;
  }

  inline void SetFP(int category,int value) {
    categoryTable[category][RES_FALSE][RES_POSITIVE] = value;
  }

  inline void SetTN(int category,int value) {
    categoryTable[category][RES_TRUE][RES_NEGATIVE] = value;
  }
  
  inline void SetFN(int category,int value) {
    categoryTable[category][RES_FALSE][RES_NEGATIVE] = value;
  }

  inline void AddTP(int category) {
    ++(categoryTable[category][RES_TRUE][RES_POSITIVE]);
    ++(globalTable[RES_TRUE][RES_POSITIVE]);
  }
 
  inline void AddTN(int category) {
    ++(categoryTable[category][RES_TRUE][RES_NEGATIVE]);
    ++(globalTable[RES_TRUE][RES_NEGATIVE]);
  }
  
  inline void AddFP(int category) {
    ++(categoryTable[category][RES_FALSE][RES_POSITIVE]);
    ++(globalTable[RES_FALSE][RES_POSITIVE]);
  }
  
  inline void AddFN(int category) {
    ++(categoryTable[category][RES_FALSE][RES_NEGATIVE]);
    ++(globalTable[RES_FALSE][RES_NEGATIVE]);
  }
  
  inline int GetTP() {
    return globalTable[RES_TRUE][RES_POSITIVE];
  }

  inline int GetTN() {
    return globalTable[RES_TRUE][RES_NEGATIVE];
  }

  inline int GetFP() {
    return globalTable[RES_FALSE][RES_POSITIVE];
  }

  inline int GetFN() {
    return globalTable[RES_FALSE][RES_NEGATIVE];
  }

  inline int GetTP(int category) {
    return categoryTable[category][RES_TRUE][RES_POSITIVE];
  }

  inline int GetTN(int category) {
    return categoryTable[category][RES_TRUE][RES_NEGATIVE];
  }

  inline int GetFP(int category) {
    return categoryTable[category][RES_FALSE][RES_POSITIVE];
  }

  inline int GetFN(int category) {
    return categoryTable[category][RES_FALSE][RES_NEGATIVE];
  }

  static Evaluation * Read(std::istream & in);

  void Write(std::ostream & out);


  inline double GetMicroAccuracy() {
    return (globalTable[RES_TRUE][RES_POSITIVE]+globalTable[RES_TRUE][RES_NEGATIVE])/
      (double)(globalTable[RES_TRUE][RES_POSITIVE]+globalTable[RES_FALSE][RES_POSITIVE]+
	       globalTable[RES_TRUE][RES_NEGATIVE]+globalTable[RES_FALSE][RES_NEGATIVE]);
  }
  
  inline double GetMicroError() {
    return (globalTable[RES_FALSE][RES_POSITIVE]+globalTable[RES_FALSE][RES_NEGATIVE])/
      (double)(globalTable[RES_TRUE][RES_POSITIVE]+globalTable[RES_FALSE][RES_POSITIVE]+
	       globalTable[RES_TRUE][RES_NEGATIVE]+globalTable[RES_FALSE][RES_NEGATIVE]);
  }

  inline double GetMicroPrecision() {
    return globalTable[RES_TRUE][RES_POSITIVE]/
      (double)(globalTable[RES_TRUE][RES_POSITIVE]+globalTable[RES_FALSE][RES_POSITIVE]);
  }
  
  inline double GetMicroRecall() {
    return globalTable[RES_TRUE][RES_POSITIVE]/
      (double)(globalTable[RES_TRUE][RES_POSITIVE]+globalTable[RES_FALSE][RES_NEGATIVE]);
  }
  
  inline double GetMicroF1() {
    return (2.0*globalTable[RES_TRUE][RES_POSITIVE])/
      (double)((2.0*globalTable[RES_TRUE][RES_POSITIVE])+
	       globalTable[RES_FALSE][RES_POSITIVE]+
	       globalTable[RES_FALSE][RES_NEGATIVE]);
  }

  inline double GetAccuracy(int k) {
    return (categoryTable[k][RES_TRUE][RES_POSITIVE]+categoryTable[k][RES_TRUE][RES_NEGATIVE])/
      (double)(categoryTable[k][RES_TRUE][RES_POSITIVE]+categoryTable[k][RES_FALSE][RES_POSITIVE]+
	       categoryTable[k][RES_TRUE][RES_NEGATIVE]+categoryTable[k][RES_FALSE][RES_NEGATIVE]);
  }
  
  inline double GetError(int k) {
    return (categoryTable[k][RES_FALSE][RES_POSITIVE]+categoryTable[k][RES_FALSE][RES_NEGATIVE])/
      (double)(categoryTable[k][RES_TRUE][RES_POSITIVE]+categoryTable[k][RES_FALSE][RES_POSITIVE]+
	       categoryTable[k][RES_TRUE][RES_NEGATIVE]+categoryTable[k][RES_FALSE][RES_NEGATIVE]);
  }
  
  inline double GetPrecision(int k) {
    double den = categoryTable[k][RES_TRUE][RES_POSITIVE]+categoryTable[k][RES_FALSE][RES_POSITIVE];
    return (den!=0)?categoryTable[k][RES_TRUE][RES_POSITIVE]/den:-1.0;
  }
  
  inline double GetRecall(int k) {
    double den = categoryTable[k][RES_TRUE][RES_POSITIVE]+categoryTable[k][RES_FALSE][RES_NEGATIVE];
    return (den!=0)?categoryTable[k][RES_TRUE][RES_POSITIVE]/den:-1.0;
  }

  inline double GetF1(int k) {
    double den = (double)((2.0*categoryTable[k][RES_TRUE][RES_POSITIVE])+
			  categoryTable[k][RES_FALSE][RES_POSITIVE]+
			  categoryTable[k][RES_FALSE][RES_NEGATIVE]);
    double num = (double)(2.0*categoryTable[k][RES_TRUE][RES_POSITIVE]);
    return (den!=0)?num/den:-1.0;
  }
  
  double GetMacroAccuracy();
  
  double GetMacroError();
  
  double GetMacroPrecision();
  
  double GetMacroRecall();
  
  double GetMacroF1();

 private:
  int categoryCount;
  int globalTable[2][2];
  int *** categoryTable;
};

#endif // _EVALUATION_H_
