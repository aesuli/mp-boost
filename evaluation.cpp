/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include "evaluation.h"

Evaluation::Evaluation(int categoryCount): categoryCount(categoryCount) {
	categoryTable = new int ** [categoryCount];
	for(int i = 0;i<categoryCount;++i) {
		categoryTable[i] = new int * [2];
		categoryTable[i][0] = new int [2];
		categoryTable[i][1] = new int [2];
	}
	Clear();
}

Evaluation::~Evaluation() {
	for(int i = 0;i<categoryCount;++i) {
		delete categoryTable[i][0];
		delete categoryTable[i][1];
		delete categoryTable[i];
	}
	delete categoryTable;
}

void Evaluation::Merge(Evaluation * evaluation) {
	for(int i = 0;i<categoryCount;++i) {
		categoryTable[i][0][0] += evaluation->categoryTable[i][0][0];
		categoryTable[i][0][1] += evaluation->categoryTable[i][0][1];
		categoryTable[i][1][0] += evaluation->categoryTable[i][1][0];
		categoryTable[i][1][1] += evaluation->categoryTable[i][1][1];
	}
	globalTable[0][0] += evaluation->globalTable[0][0];
	globalTable[0][1] += evaluation->globalTable[0][1];
	globalTable[1][0] += evaluation->globalTable[1][0];
	globalTable[1][1] += evaluation->globalTable[1][1];
}

void Evaluation::Clear() {
	for(int i = 0;i<categoryCount;++i) {
		categoryTable[i][0][0] = 0;
		categoryTable[i][0][1] = 0;
		categoryTable[i][1][0] = 0;
		categoryTable[i][1][1] = 0;
	}
	globalTable[0][0] = 0;
	globalTable[0][1] = 0;
	globalTable[1][0] = 0;
	globalTable[1][1] = 0;
}

Evaluation * Evaluation::Read(std::istream & in) {
	int glob[2][2];
	in >> glob[RES_TRUE][RES_POSITIVE];
	in >> glob[RES_TRUE][RES_NEGATIVE];
	in >> glob[RES_FALSE][RES_POSITIVE];
	in >> glob[RES_FALSE][RES_NEGATIVE];

	int categoriesCount;
	in >> categoriesCount;

	Evaluation * res = new Evaluation(categoriesCount);
	res->globalTable[RES_TRUE][RES_POSITIVE] = glob[RES_TRUE][RES_POSITIVE];
	res->globalTable[RES_TRUE][RES_NEGATIVE] = glob[RES_TRUE][RES_NEGATIVE];
	res->globalTable[RES_FALSE][RES_POSITIVE] = glob[RES_FALSE][RES_POSITIVE];
	res->globalTable[RES_FALSE][RES_NEGATIVE] = glob[RES_FALSE][RES_NEGATIVE];

	for(int i = 0;i<categoriesCount;++i) {
		in >> res->categoryTable[i][RES_TRUE][RES_POSITIVE];
		in >> res->categoryTable[i][RES_TRUE][RES_NEGATIVE];
		in >> res->categoryTable[i][RES_FALSE][RES_POSITIVE];
		in >> res->categoryTable[i][RES_FALSE][RES_NEGATIVE];
	}
	return res;
}

void Evaluation::Write(std::ostream & out) {
	out << globalTable[RES_TRUE][RES_POSITIVE] << ' ';
	out << globalTable[RES_TRUE][RES_NEGATIVE] << ' ';
	out << globalTable[RES_FALSE][RES_POSITIVE] << ' ';
	out << globalTable[RES_FALSE][RES_NEGATIVE] << std::endl;

	out << categoryCount << std::endl;
	for(int i = 0;i<categoryCount;++i) {
		out << categoryTable[i][RES_TRUE][RES_POSITIVE] << ' ';
		out << categoryTable[i][RES_TRUE][RES_NEGATIVE] << ' ';
		out << categoryTable[i][RES_FALSE][RES_POSITIVE] << ' ';
		out << categoryTable[i][RES_FALSE][RES_NEGATIVE] << std::endl;
	}
}

double Evaluation::GetMacroAccuracy() {
	double MP = 0.0;
	for(int i = 0;i<categoryCount;++i) {
		double p = GetAccuracy(i);
		if(p>=0)
		  MP += p;
	}
	return MP/categoryCount;
}

double Evaluation::GetMacroError() {
	double MP = 0.0;
	for(int i = 0;i<categoryCount;++i) {
		double p = GetError(i);
		if(p>=0) 
		  MP += p;
	}
	return MP/categoryCount;
}

double Evaluation::GetMacroPrecision() {
	double MP = 0.0;
	for(int i = 0;i<categoryCount;++i) {
		double p = GetPrecision(i);
		if(p>=0)
			MP += p;
	}
	return MP/categoryCount;
}

double Evaluation::GetMacroRecall() {
	double MR = 0.0;
	for(int i = 0;i<categoryCount;++i) {
		double r = GetRecall(i);
		if(r>=0)
			MR += r;
	}
	return MR/categoryCount;
}

double Evaluation::GetMacroF1() {
	double MR = 0.0;
	for(int i = 0;i<categoryCount;++i) {
		double r = GetF1(i);
		if(r>=0)
		  MR += r;
	}
	return MR/categoryCount;
}
