/*
 * MDP.hpp
 *
 *	An c++ implementation of a Markov Decision Process (MDP)
 *
 *  Created on: Jul 12, 2015
 *      Author: alexminnaar
 */

#ifndef MDP_MDP_HPP_
#define MDP_MDP_HPP_

#include <boost/numeric/ublas/matrix.hpp>
#include <map>
#include <boost/numeric/ublas/vector.hpp>
#include <MDP/storage_adaptors.hpp>
#include <boost/numeric/ublas/io.hpp>

#define N_STATES 16

using namespace boost::numeric;


class MDP{

private :
	//mapping from action to probability transition matrix
	std::map<int,ublas::matrix<double> > actionTransitions;

	//ublas::matrix where entry (i,j) is the reward associated with taking action j from state j
	ublas::matrix<double> actionReward;

	//MDP discount factor in [0,1]
	double discount;

	//Total number of states in MPD
	int numStates;

	//Total number of actions in MDP
	int numActions;

	//Constructor initializing all member variables
	MDP(std::map<int,ublas::matrix<double> > at, ublas::matrix<double> ar, double d);

public:

    MDP(int n_interfaces, double d);
	//reward for each state associated with this policy (given the action reward and transition ublas::matrix for this MDP)
	ublas::vector<double> policyReward(ublas::matrix<double> policy);

	//transition ublas::matrix associated with this policy (given the transition ublas::matrix for this MDP)
	ublas::matrix<double> policyTransitions(ublas::matrix<double> policy);

	//compute the Bellman equation for the given parameters
	ublas::vector<double> bellmanEquation(ublas::matrix<double> policyTrans, ublas::vector<double> policyRew, ublas::vector<double> valueFunc);

	//Compute the value function associated with a given policy
	ublas::vector<double> policyEvaluation(ublas::matrix<double> policyTrans, ublas::vector<double> policyRew, double epsilon);

	//Greedy policy improvement given the current policy's value function
	ublas::matrix<double> policyImprovement(ublas::vector<double> valueFunction);

	//compute the optimal policy for this MDP (corresponding value function can be found using policyEvalution method)
	ublas::matrix<double> policyIteration();

};

#endif /* MDP_HPP_ */
