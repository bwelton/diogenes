/**
 * \file cuIBM.cu
 * \brief Main source file of \c cuIBM.
 */


#include "utilities/types.h"
#include "utilities/helpers.h"
#include "utilities/domain.h"
#include "io/io.h"
#include "solvers/createSolver.h"
#include <map>
#include <vector>
std::map<uint64_t, std::vector<void *> > MY_CUSTOM_MALLOCMM;


/**
 * \brief Main function.
 */
int main(int argc, char **argv)
{
	// initialize the computational domain
	domain dom_info;

	// initialize the parameters of the simulation
	parameterDB paramDB;

	// read input files
	io::readInputs(argc, argv, paramDB, dom_info);
	
	io::printSimulationInfo(paramDB, dom_info);

	// create and initialize the appropriate flow solver
	NavierStokesSolver<device_memory> *solver = createSolver<device_memory>(paramDB, dom_info);
	solver->initialise();

	io::printDeviceMemoryUsage(std::string("Initialization complete"));
	
	// time-step loop
	while (!solver->finished())
	{
		solver->stepTime();
		solver->writeData();
	}

	solver->shutDown();
} // main
