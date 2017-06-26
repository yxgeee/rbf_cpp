#pragma once

#include<vector>
#include<math.h>
#include "KmeansPP.h"
#include "Util.h"


typedef std::vector<double> datapoint;
class RBFNetwork
{
public:
	RBFNetwork(const std::vector<datapoint> &training_data, const std::vector<datapoint> &training_labels, const std::vector<datapoint> &testing_data, const std::vector<datapoint> &testing_labels, int num_of_labels);
	~RBFNetwork(void);

	/* Start Training the Radial Basis Function network
		Takes the number of RBF centroids, the learning rate, the number of iteration and a print flag as input
		Saves the output model to be used in testing and single predictions 
		return accuracy and mse (by reference)
		*/
	std::vector<datapoint> startTraining(int num_rbf_units, double learning_rate, int num_iterations, double &mse, bool print_flag = false);

	/* Start Testing the RBF Network to make sure it's not overfitting
		(Should be done after training) */
	std::vector<datapoint> startTesting(const std::vector<datapoint> &test_data, const std::vector<datapoint> &test_labels, double &mse);

	
	/* Predict a single data point support multi-classes (One vs. All method)
		(Should be done after training of course) */
	datapoint predictLabel(const datapoint &data_point, const datapoint &data_label, double &error);

	//TODO
	void saveModel();
	void loadModel();

private:
	int num_of_labels;
	double gamma;
	std::vector<datapoint> training_data;
	std::vector<datapoint> training_labels;
	std::vector<datapoint> testing_data;
	std::vector<datapoint> testing_labels;
	std::vector< std::vector<double> > rbf_units;
	std::vector< std::vector<double> > layer2_weights;
	std::vector<datapoint> rbf_centroids;
	//std::vector<double>total_centroids_dist;

	// Random Number seed devices/engines/distributions
    std::random_device rd;
	std::default_random_engine random_engine;
	std::uniform_real_distribution<double> random_real_gen;

	void buildRBFUnits();
	void calculateGamma();
	double distance(const datapoint &a ,const datapoint &b);
	double basisFunction(const datapoint &data_point, const datapoint &centroid);
	
};

