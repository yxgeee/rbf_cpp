#include <stdio.h>
#include <string>
#include <algorithm>
#include <time.h>

#include "RBFNetwork.h"

using namespace std;

string data_dir = "data.xls";

// string intToStr(int num)
// {
// 	if(num==0)return "0";
// 	string ret = "";
// 	while(num>0)
// 	{
// 		ret += (num%10)+'0';
// 		num/=10;
// 	}
// 	reverse(ret.begin(),ret.end());
// 	return ret;
// }

// datapoint imageToDoubleVec(cv::Mat image)
// {
// 	Mat image_gray;
// 	cvtColor(image,image_gray,CV_RGB2GRAY);
// 	equalizeHist( image_gray, image_gray );
// 	datapoint data_point(image_gray.rows*image_gray.cols);
// 	int i=0;
// 	MatConstIterator_<uchar> it = image_gray.begin<uchar>(), it_end = image_gray.end<uchar>();
// 	for(; it != it_end; ++it)
// 		data_point[i++] = ((double)(*it)/128.0)-1;
// 	return data_point;
// }

// __inline void loadTrainingData(vector<datapoint> &training_data, vector<int> &training_labels, int training_size)
// {
// 	string training_dir = data_dir+"Training/";
// 	// Front
// 	for(int i = 0 ; i<training_size ; i++)
// 	{
// 		Mat image = imread(training_dir+"Data_Front/head (" + intToStr(i+1) + ").png");
// 		if(!image.rows)
// 		{
// 			printf("Error Loading Image!\n");
// 			continue;
// 		}
// 		training_data.push_back(imageToDoubleVec(image));
// 		training_labels.push_back(0);
// 	}

// 	// Left
// 	for(int i = 0 ; i<training_size ; i++)
// 	{
// 		Mat image = imread(training_dir+"Data_Left/head (" + intToStr(i+1) + ").png");
// 		if(!image.rows)
// 		{
// 			printf("Error Loading Image!\n");
// 			continue;
// 		}
// 		training_data.push_back(imageToDoubleVec(image));
// 		training_labels.push_back(1);
// 	}

// 	// Right
// 	for(int i = 0 ; i<training_size ; i++)
// 	{
// 		Mat image = imread(training_dir+"Data_Right/head (" + intToStr(i+1) + ").png");
// 		if(!image.rows)
// 		{
// 			printf("Error Loading Image!\n");
// 			continue;
// 		}
// 		training_data.push_back(imageToDoubleVec(image));
// 		training_labels.push_back(2);
// 	}
// 	vector<int>perm(training_data.size());
// 	vector<datapoint> temp = training_data;
// 	vector<int> temp2 = training_labels;
// 	for(int i =0 ;i<training_data.size() ; i++)	perm[i]=i;
// 	srand (unsigned(time(0)));
// 	random_shuffle(perm.begin(),perm.end());
// 	for(int i =0 ;i<training_data.size() ; i++) training_data[perm[i]] = temp[i],training_labels[perm[i]] = temp2[i];
// }

// __inline void loadTestingData(vector<datapoint> &testing_data, vector<int> &testing_labels, int testing_size)
// {
// 	string testing_dir = data_dir+"Testing/";
// 	// Front
// 	for(int i = 0 ; i<testing_size ; i++)
// 	{
// 		Mat image = imread(testing_dir+"Test_Front/head (" + intToStr(i+1) + ").png");
// 		if(!image.rows)
// 		{
// 			printf("Error Loading Image!\n");
// 			continue;
// 		}
// 		testing_data.push_back(imageToDoubleVec(image));
// 		testing_labels.push_back(0);
// 	}

// 	// Left
// 	for(int i = 0 ; i<testing_size ; i++)
// 	{
// 		Mat image = imread(testing_dir+"Test_Left/head (" + intToStr(i+1) + ").png");
// 		if(!image.rows)
// 		{
// 			printf("Error Loading Image!\n");
// 			continue;
// 		}
// 		testing_data.push_back(imageToDoubleVec(image));
// 		testing_labels.push_back(1);
// 	}

// 	// Right
// 	for(int i = 0 ; i<testing_size ; i++)
// 	{
// 		Mat image = imread(testing_dir+"Test_Right/head (" + intToStr(i+1) + ").png");
// 		if(!image.rows)
// 		{
// 			printf("Error Loading Image!\n");
// 			continue;
// 		}
// 		testing_data.push_back(imageToDoubleVec(image));
// 		testing_labels.push_back(2);
// 	}
// }



int main()
{
	vector<datapoint> training_data, testing_data;
	vector<datapoint> training_labels, testing_labels;
	datapoint a[5]={1.2,2.7,3.0,4.7,5.5};
	datapoint b[1]={9.2};
	// loadTrainingData(training_data,training_labels, 50);
	// loadTestingData(testing_data,test_labels, 30);
	training_data.push_back(a);
	training_data.push_back(a);
	training_data.push_back(a);
	testing_data.push_back(a);
	testing_data.push_back(a);

	training_labels.push_back(b);
	training_labels.push_back(b);
	training_labels.push_back(b);
	testing_labels.push_back(b);
	testing_labels.push_back(b);

	RBFNetwork RBFNN(training_data, training_labels, 1);
	double mse=0;
	RBFNN.startTraining(11, 0.005, 10, mse, true);

	// Experimenting results for different number of RBF units and learning rates
	// for(int rbf_units = 3 ; rbf_units<=100 ; rbf_units+=2)
	// {
	// 	for(double learning = 0.01 ; learning<=10.0 ; learning*=2.0)
	// 	{
	// 		printf("RBF Network with %d units, learning rate=%f\n", rbf_units, learning);
	// 		double mse=0;
	// 		double acc  = RBFNN.startTraining(rbf_units, learning, 10, mse, true);
	// 		RBFNN.startTesting(testing_data,test_labels);
	// 		//printf(" Acc=%.6f , MSE=%.6f\n", acc, mse);
	// 	}
	// }

	return 0;
}