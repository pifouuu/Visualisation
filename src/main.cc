/*
 * displayResults.cpp
 *
 *  Created on: 27 févr. 2017
 *      Author: pierre
 */

#include <vector>
#include <cmath>
// include input and output archivers
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// include this header to serialize vectors
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include "../include/gnuplot-iostream.h"

int main() {
	Gnuplot gp;

	std::vector<std::vector<std::pair<float,float>>> act_try(17);
	for (int i=0; i<17; i++){
		std::ifstream ifs("../myTexplore/act_try_"+std::to_string(i)+".ser");
		boost::archive::text_iarchive ia(ifs);
		ia & act_try[i];
	}

	gp << "set xrange [0:100000]\nset yrange [0:20000]\n";
	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	gp << "plot";
	for (int i=0;i<17;i++){
		gp << gp.file1d(act_try[i]) << "with lines title 'act_try_" << std::to_string(i)<<"',";
	}
	gp << std::endl;

	std::vector<std::vector<std::pair<float,float>>> act_acc(17);
	for (int i=0; i<17; i++){
		std::ifstream ifs("../myTexplore/act_acc_"+std::to_string(i)+".ser");
		boost::archive::text_iarchive ia(ifs);
		ia & act_acc[i];
	}

	gp << "set xrange [0:100]\nset yrange [0:1]\n";
	gp << "set terminal x11 1\n";
	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	gp << "plot";
	for (int i=0;i<17;i++){
		gp << gp.file1d(act_acc[i]) << "with lines title 'act_try_" << std::to_string(i)<<"',";
	}
	gp << std::endl;

	std::vector<std::vector<std::pair<float,float>>> act_suc(17);
	for (int i=0; i<17; i++){
		std::ifstream ifs("../myTexplore/act_succes_"+std::to_string(i)+".ser");
		boost::archive::text_iarchive ia(ifs);
		ia & act_suc[i];
	}

	gp << "set xrange [0:100000]\nset yrange [0:1]\n";
	gp << "set terminal x11 2\n";

	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	gp << "plot";
	for (int i=0;i<17;i++){
		gp << gp.file1d(act_suc[i]) << "with impulses title 'act_succes_" << std::to_string(i)<<"',";
	}
	gp << std::endl;
}


