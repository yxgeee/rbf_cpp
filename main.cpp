#include <stdio.h>
#include <string>
#include <algorithm>
#include <time.h>

#include "RBFNetwork.h"
#include "BasicExcel.hpp"

using namespace YExcel;
using namespace std;

int main()
{
	vector<datapoint> training_data, testing_data;
	vector<datapoint> training_labels, testing_labels;
	int num_predict=4;

	BasicExcel e;
	e.Load("data.xls");
  	BasicExcelWorksheet* sheet1 = e.GetWorksheet("Sheet1");
  	if(sheet1)
  	{
  		size_t maxRows = sheet1->GetTotalRows();
    	size_t maxCols = sheet1->GetTotalCols()-4;

        for (size_t r=0; r<maxRows; ++r)
    	{
    		if (r<(size_t)maxRows*0.9)
    		{
    			datapoint train_in,train_out;
    			for (size_t c=0;c<maxCols-num_predict;++c)
    			{
    				BasicExcelCell* cell = sheet1->Cell(r,c);
    				train_in.push_back(cell->GetDouble());
    			}
    			for (size_t c=maxCols-num_predict;c<maxCols;++c)
    			{
    				BasicExcelCell* cell = sheet1->Cell(r,c);
    				train_out.push_back(cell->GetDouble());
    			}
    			training_data.push_back(train_in);
    			training_labels.push_back(train_out);
    		}else{
    			datapoint test_in,test_out;
    			for (size_t c=0;c<maxCols-num_predict;++c)
    			{
    				BasicExcelCell* cell = sheet1->Cell(r,c);
    				test_in.push_back(cell->GetDouble());
    			}
    			for (size_t c=maxCols-num_predict;c<maxCols;++c)
    			{
    				BasicExcelCell* cell = sheet1->Cell(r,c);
    				test_out.push_back(cell->GetDouble());
    			}
    			testing_data.push_back(test_in);
    			testing_labels.push_back(test_out);
    		}
      	}
  	}

  	//shuffle
  	vector<int>perm(training_data.size());
	vector<datapoint> temp = training_data;
	vector<datapoint> temp2 = training_labels;
	for(int i =0 ;i<training_data.size() ; i++)	perm[i]=i;
	srand (unsigned(time(0)));
	random_shuffle(perm.begin(),perm.end());
	for(int i =0 ;i<training_data.size() ; i++) training_data[perm[i]] = temp[i],training_labels[perm[i]] = temp2[i];

	RBFNetwork RBFNN(training_data, training_labels, testing_data, testing_labels, num_predict);
	// Experimenting results for different number of RBF units and learning rates
	double bestmse=1e7;
	int bestunits;
	double bestlr;
	vector<datapoint> best_prediction;
	for(int rbf_units = 3 ; rbf_units<=50 ; rbf_units+=2)
	{
		for(double learning = 0.001 ; learning<=0.2 ; learning*=2.0)
		{
			printf("RBF Network with %d units, learning rate=%f\n", rbf_units, learning);
			double mse=0;
			vector<datapoint> prediction = RBFNN.startTraining(rbf_units, learning, 50, mse, true);
			// double test_mse=0;
			// vector<datapoint> prediction = RBFNN.startTesting(testing_data,testing_labels,test_mse);
			if (mse<bestmse)
			{
				bestmse = mse;
				bestunits = rbf_units;
				bestlr = learning;
				best_prediction = prediction;
			}
		}
	}

	printf("The Best RBF Network: units=%d, learning_rate=%f, test_mse=%f\n", bestunits,bestlr,bestmse);
  	BasicExcelWorksheet* sheet = e.AddWorksheet("Predict", 1);
  	sheet = e.GetWorksheet(1);
  	if(sheet)
  	{
  		for(size_t r=0; r<best_prediction.size(); ++r)
  		{
  			for (size_t c=0;c<best_prediction[r].size();++c)
  			{
  				sheet->Cell(r,c)->SetDouble(best_prediction[r][c]);
  			}
  		}
  	}
  	sheet = e.AddWorksheet("Real", 2);
  	sheet = e.GetWorksheet(2);
  	if(sheet)
  	{
  		for(size_t r=0; r<testing_labels.size(); ++r)
  		{
  			for (size_t c=0;c<testing_labels[r].size();++c)
  			{
  				sheet->Cell(r,c)->SetDouble(testing_labels[r][c]);
  			}
  		}
  	}
  	e.SaveAs("predict.xls");
	return 0;
}