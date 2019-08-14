//============================================================================
// Name        : MDP.cpp
// Author      : Alex Minnaar
// Description : An c++ implementation of a Markov Decision Process (MDP)
//============================================================================
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <map>
#include <boost/numeric/ublas/io.hpp>
#include <MDP/MDP.hpp>
#include <MDP/storage_adaptors.hpp>
#include <float.h>



using namespace boost::numeric;

/**
  * Prototypes
  */
ublas::matrix<double> initializeReceivingActionTransition(void);
ublas::matrix<double> initializeForwardingActionTransition(void);
ublas::matrix<double> initializeRewards(int n_actions);


MDP::MDP(int n_interfaces, double d)
{    
    std::map<int, ublas::matrix<double>> at;
    at[0] = initializeReceivingActionTransition();
    ublas::matrix<double> mat = initializeForwardingActionTransition();

    for(int i = 1; i <= n_interfaces; i++)
    {
        at[i] = mat;
    }

    ublas::matrix<double> ar = initializeRewards(n_interfaces + 1);

    this->actionTransitions = at;
    this->actionReward = ar;
    this->discount = d;
    this->numStates = ar.size1();
	this->numActions = at.size();
}

//Constructor initializing all member variables
MDP::MDP(std::map<int, ublas::matrix<double> > at, ublas::matrix<double> ar, double d) {
	this->actionTransitions = at;
	this->actionReward = ar;
	this->discount = d;
	this->numStates = ar.size1();
	this->numActions = at.size();
}

//reward for each state associated with this policy (given the action reward and transition ublas::matrix for this MDP)
ublas::vector<double> MDP::policyReward(ublas::matrix<double> policy) {

	ublas::vector<double> v(this->actionReward.size1());

	for (unsigned i = 0; i < policy.size1(); ++i) {

		ublas::matrix_row<ublas::matrix<double> > policyRow(policy, i);
		ublas::matrix_row<ublas::matrix<double> > rewardRow(this->actionReward, i);

		v(i) = ublas::sum(ublas::element_prod(policyRow, rewardRow));
	}

	return v;
}

//transition matrix associated with this policy (given the transition matrix for this MDP)
ublas::matrix<double> MDP::policyTransitions(ublas::matrix<double> policy) {

	ublas::matrix<double> ptp = ublas::zero_matrix<double>(3, 3);

	for (unsigned i = 0; i < policy.size1(); ++i) {

		for (std::map<int, ublas::matrix<double> >::iterator it =
				this->actionTransitions.begin();
				it != this->actionTransitions.end(); ++it) {

			ublas::matrix_row<ublas::matrix<double> > atmRow(it->second, i);

			row(ptp, i) += atmRow * policy(i, it->first);
		}

	}

	return ptp;
}

//Compute the result of the Bellman equation
ublas::vector<double> MDP::bellmanEquation(ublas::matrix<double> policyTrans,
		ublas::vector<double> policyRew, ublas::vector<double> valueFunc) {

	return policyRew + this->discount * prod(policyTrans, valueFunc);
}

//Compute the value function associated with a given policy
ublas::vector<double> MDP::policyEvaluation(ublas::matrix<double> pTransProb,
		ublas::vector<double> pReward, double epsilon) {

	//Initialize value function to zero
	ublas::vector<double> valueFunction = ublas::zero_vector<double>(pReward.size());

	double delta = 10.0;

	while (delta > epsilon) {

		ublas::vector<double> previousValueFunction = valueFunction;

		//compute value function via the Bellman equation
		valueFunction = bellmanEquation(pTransProb, pReward, valueFunction);

		//Check for convergence
		ublas::vector<double> diffVect = valueFunction - previousValueFunction;

		//get maximum element
		ublas::vector<double>::iterator result;

		//delta is most changed element (if it is small then convergence has occurred)
		delta = *std::max_element(diffVect.begin(), diffVect.end());
	}

	return valueFunction;
}

struct actionValue {
	int action;
	double value;
};

//Greedy policy improvement given the current policy's value function
ublas::matrix<double> MDP::policyImprovement(ublas::vector<double> valueFunction) {

	ublas::matrix<double> greedyPolicy = ublas::zero_matrix<double>(this->numStates,
			this->numActions);

	for (int i = 0; i < numStates; ++i) {

		actionValue greedyAction = { -1, 0.0 };

		for (std::map<int, ublas::matrix<double> >::iterator it =
				this->actionTransitions.begin();
				it != this->actionTransitions.end(); ++it) {

			//compute the value associated with this action at this state
			double value = inner_prod(row(it->second, i), valueFunction)
					+ this->actionReward(i, it->first);

			//select the greedy action in terms of value
			if (value > greedyAction.value) {
				greedyAction.action = it->first;
				greedyAction.value = value;
			}
		}

		greedyPolicy(i, greedyAction.action) = 1.0;
	}

	return greedyPolicy;
}

//compute the optimal policy for this MDP (corresponding value function can be found using policyEvalution method)
ublas::matrix<double> MDP::policyIteration() {

	//initialize with a random policy
	ublas::matrix<double> currentPolicy = ublas::scalar_matrix<double>(this->numStates,
			this->numActions);

	currentPolicy = currentPolicy / (double) this->numActions;

	ublas::matrix<double> oldPolicy = ublas::zero_matrix<double>(this->numStates,
			this->numActions);

	const double epsilon = std::numeric_limits<double>::epsilon();

	while (!ublas::detail::equals(currentPolicy, oldPolicy, epsilon, epsilon)) {

		oldPolicy = currentPolicy;

		ublas::matrix<double> policyTrans = this->policyTransitions(currentPolicy);

		ublas::vector<double> policyReward = this->policyReward(currentPolicy);

		ublas::vector<double> policyValue = this->policyEvaluation(policyTrans,
				policyReward, 0.001);

		currentPolicy = this->policyImprovement(policyValue);
	}

	return currentPolicy;
}


void MDP::updateRewards(std::vector<float> rtts)
{
    ublas::matrix<double> mat(N_STATES, numActions);

    for(int i = 0; i < N_STATES; i++)
    {
        mat(i, 0) = 0;
        for(int j = 1; j < numActions; j--)
        {
            mat(i, j) = FLT_MAX - rtts[j-1];
        }
    }
    this->actionReward = mat;
}

/***************************************
 *  Code that initializes The Matrices *
 ***************************************/


int countOnes(int n)
{
    int n_bits = (int)log2(N_STATES);

    int count = 0; 
    for(int i = 0; i < n_bits; i++)
    {
        if(n & (1 << i))
        {
            ++count;
        }
    }
    return count;
}

int countZeros(int n)
{
    int n_bits = (int)log2(N_STATES);

    int count = 0; 
    for(int i = 0; i < n_bits; i++)
    {
        if(!(n & (1 << i)))
        {
            ++count;
        }
    }
    return count;
}

bool isPower2(double n)
{
    double p = log2(n);
    return p == floor(p);
}
ublas::matrix<double> initializeForwardingActionTransition(void)
{
    
    ublas::matrix<double> mat(N_STATES, N_STATES);

    for(int i = 0; i < N_STATES; i++)
    {
        double probability = 1.0 / (countOnes(i) + 1.0);        
        for(int j = i; j >= 0; j--)
        {
            if(j == i || (isPower2(i - j) && ((j | i) == i)))
            {
                mat(i, j) = probability;
            }
            else
            {
                mat(i, j) = 0;
            }
        }               
    }
    return mat;
}

ublas::matrix<double> initializeReceivingActionTransition(void)
{
    
    ublas::matrix<double> mat(N_STATES, N_STATES);

    for(int i = 0; i < N_STATES; i++)
    {
        double probability;
        if(i == 0)
        {
            probability = 1.0 / (countZeros(i));        
        }
        else
        {
            probability = 1.0 / (countZeros(i) + 1);
        }

        for(int j = i; j < N_STATES; j++)
        {
            if((j > 0) && ((j == i) || (isPower2(j - i) && ((i | j) == j))))
            {
                mat(i, j) = probability;
            }
            else
            {
                mat(i, j) = 0;
            }
        }               
    }
    return mat;
}

ublas::matrix<double> initializeRewards(int n_actions)
{
    ublas::matrix<double> mat(N_STATES, n_actions);

    for(int i = 0; i < N_STATES; i++)
    {
        for(int j = 1; j < n_actions; j--)
        {
            mat(i, j) = DBL_MAX;
        }
    }
    
    return mat;
}





