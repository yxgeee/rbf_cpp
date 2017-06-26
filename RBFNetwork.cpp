#include "RBFNetwork.h"

using namespace std;

RBFNetwork::RBFNetwork(const vector<datapoint> &training_data, const vector<datapoint> &training_labels, int num_of_labels)
	: training_data(training_data), training_labels(training_labels), num_of_labels(num_of_labels)
	, random_real_gen(-1, 1), random_engine(rd())
{}

RBFNetwork::~RBFNetwork(void)
{
}

void RBFNetwork::startTraining(int num_rbf_units, double learning_rate, int num_iterations, double &mse, bool print_flag)
{
	if(print_flag)
	{
		printf("Starting RBF Network Training with %d units and learning rate=%f...\n", num_rbf_units, learning_rate);
		printf("Getting RBF Centroids using K-means++...\n");
	}


	// Calculate RBF Centroids
	KmeansPP KMPP(training_data);
	rbf_centroids.clear();
	KMPP.RunKMeansPP(num_rbf_units, rbf_centroids);
	calculateGamma();

	if(print_flag)
		printf("Building the RBF Units...\n");
	// Build First (Fixed) Layer Units
	buildRBFUnits();

	// Init the Second Layer with values [-1,1]
	layer2_weights.assign(num_of_labels, vector<double>(num_rbf_units,0));
	for(auto &label: layer2_weights)
		for(auto &arc: label)
			arc = random_real_gen(random_engine);

	// Train the second layer weights
	mse = 0;
	// double accuracy=0;
	for(int iter = 0 ; iter<num_iterations ; iter++)
	{	
		//prediction
		for(int i = 0 ; i<training_data.size() ; i++)
		{
			for(int label = 0 ; label<num_of_labels ; label++)
			{
				double hypothesis = Utility::multiplyVectors(rbf_units[i], layer2_weights[label]);
				double error_direction = (training_labels[i][label]-hypothesis);
				vector<double> delta = Utility::multiplyVecConst(rbf_units[i], error_direction * learning_rate);
				Utility::AddVectors(layer2_weights[label], delta);	
			}
		}

		if(print_flag)
		{
			// Gathering Statistics
			mse = 0.0;
			for(int i = 0 ; i<training_data.size() ; i++)
			{
				double error_dir=0;
				datapoint predict_data=predictLabel(training_data[i], training_labels[i], error_dir);
				mse += error_dir;
			}
			mse *= (double)(1.0/((double)training_data.size()*(double)num_of_labels));
			// accuracy *= (double)(1.0/(double)training_data.size());
			printf("Training (%*d/%d), MSE=[%.3f], Progress [%.2f]\r",
				2, (iter+1), num_iterations, mse, ((double)((double)(iter+1)/(double)num_iterations) * 100.0));
		}

		if(mse < 1e-9)
			break;
	}
	if(print_flag)
		printf("\n----------------------------\n");
}

void RBFNetwork::buildRBFUnits()
{
	//total_centroids_dist.clear();
	//total_centroids_dist.assign(rbf_centroids.size(),0);
	//// Calculate total distance for each centroid
	//for(int i = 0 ; i<rbf_centroids.size() ; i++)
	//	for(auto &data_point : training_data)
	//		total_centroids_dist[i] += distance(data_point,rbf_centroids[i]);

	// Build RBF Units
	rbf_units.assign(training_data.size(),vector<double>());
	for(int i = 0 ; i<training_data.size() ; i++)
	{
		for(int j = 0 ; j<rbf_centroids.size() ; j++)
			rbf_units[i].push_back(basisFunction(training_data[i], rbf_centroids[j]));
	}
}

double RBFNetwork::basisFunction(const datapoint &data_point, const datapoint &centroid)
{ 
	return exp(gamma * distance(data_point,centroid));
}

double RBFNetwork::distance(const datapoint &a, const datapoint &b)
{
	double dist=0;
	for(int i = 0 ; i< a.size() ; i++)
		dist += (a[i]-b[i])*(a[i]-b[i]);
	return dist;
}

datapoint RBFNetwork::predictLabel(const datapoint &data_point, const datapoint &data_label,double &error)
{
	vector<double> cur_rbf_unit;
	for(int j = 0 ; j<rbf_centroids.size() ; j++)
		cur_rbf_unit.push_back(basisFunction(data_point, rbf_centroids[j]));
	// Get the label with maximum hypothesis
	datapoint predict_data;
	for(int label = 0 ; label<num_of_labels ; label++)
	{
		double hypothesis = Utility::multiplyVectors(cur_rbf_unit, layer2_weights[label]);
		double error_direction = (data_label[label]-hypothesis);
		error += error_direction*error_direction;
		predict_data.push_back(hypothesis);
	}
	return predict_data;
}

void RBFNetwork::calculateGamma()
{
	double variance = -1;
	for (int i = 0; i < rbf_centroids.size() ; ++i)
		for (int j = i + 1; j < rbf_centroids.size() ; ++j)
			variance = max(variance, distance(rbf_centroids[i], rbf_centroids[j]));
	variance *= (1.0 / ((double)rbf_centroids.size()));
	gamma = (-1.0 / (2.0 * variance));
}


std::vector<datapoint> RBFNetwork::startTesting(const std::vector<datapoint> &testing_data, const std::vector<datapoint> &testing_labels, double &mse)
{
	printf("Testing...\n");
	double err=0;
	std::vector<datapoint> predict_test;
	for(int i = 0 ; i<testing_data.size() ; i++)
	{
		datapoint predict_data = predictLabel(testing_data[i],testing_labels[i],err);
		mse += err;
		predict_test.push_back(predict_data);
	}
	mse *= (double)(1.0/((double)testing_data.size()*(double)num_of_labels));	
	printf("Testing Results MSE = %.6f\n",mse);
	printf("------------------------------\n");
	return predict_test;
}