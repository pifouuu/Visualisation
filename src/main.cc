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

#define NUMACTIONS 8

int main() {
	Gnuplot gp;
	std::vector<std::string> actions;
	actions.push_back(std::string("GO_TO_EYE"));
	actions.push_back(std::string("LOOK_RED_BOX"));
	actions.push_back(std::string("LOOK_BLUE_BOX"));
	actions.push_back(std::string("PICK"));
	actions.push_back(std::string("PUT_DOWN"));
	actions.push_back(std::string("PUT_IN"));
	actions.push_back(std::string("LOOK_RED_BLOCK_0"));
	//actions.push_back(std::string("LOOK_RED_BLOCK_1"));
	actions.push_back(std::string("LOOK_BLUE_BLOCK_0"));
	//actions.push_back(std::string("LOOK_BLUE_BLOCK_1"));
	//actions.push_back(std::string("LOOK_BLUE_BLOCK_2"));

	std::vector<std::vector<std::pair<float,float>>> act_try(NUMACTIONS);
	std::string dirname = "../myTexplore/";
	std::string name = dirname+"no_tutor_v_0.000000_n_0.000000_act_try_";
	for (int i=0; i<NUMACTIONS; i++){
		std::ifstream ifs(name+actions[i]+".ser");
		boost::archive::text_iarchive ia(ifs);
		ia & act_try[i];
	}

	gp << "set xrange [0:100]\nset yrange [0:30]\n";
	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	gp << "plot";
	for (int i=3;i<NUMACTIONS;i++){
		gp << gp.file1d(act_try[i]) << "with lines title 'act_try_" << actions[i] <<"',";
	}
	gp << std::endl;


	std::vector<std::vector<std::pair<float,float>>> act_acc(NUMACTIONS);
	name = dirname+"no_tutor_v_0.000000_n_0.000000_act_acc_";
	for (int i=0; i<NUMACTIONS; i++){
		std::ifstream ifs(name+actions[i]+".ser");
		boost::archive::text_iarchive ia(ifs);
		ia & act_acc[i];
	}

	gp << "set xrange [0:3000]\nset yrange [0:1.5]\n";
	gp << "set terminal x11 1\n";
	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	gp << "plot";
	for (int i=0;i<NUMACTIONS;i++){
		gp << gp.file1d(act_acc[i]) << "with lines title 'act_acc_" << actions[i]<<"',";
	}
	gp << std::endl;


	std::vector<std::vector<std::pair<float,float>>> act_suc(NUMACTIONS);
	name = dirname+"no_tutor_v_0.000000_n_0.000000_act_succes_";
	for (int i=0; i<NUMACTIONS; i++){
		std::ifstream ifs(name+actions[i]+".ser");
		boost::archive::text_iarchive ia(ifs);
		ia & act_suc[i];
	}

	gp << "set xrange [0:5000]\nset yrange [0:20]\n";
	gp << "set terminal x11 2\n";

	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	gp << "plot";
	for (int i=0;i<NUMACTIONS;i++){
		gp << gp.file1d(act_suc[i]) << "with lines title 'act_succes_" << actions[i]<<"',";
	}
	gp << std::endl;


	std::vector<std::pair<float,float>> blocks_in;
	std::ifstream ifs(dirname+"no_tutor_v_0.000000_n_0.000000_blocks_in.ser");
	boost::archive::text_iarchive ia_in(ifs);
	ia_in & blocks_in;
	gp << "set xrange [0:5000]\nset yrange [0:1]\n";
	gp << "set terminal x11 3\n";
	gp << "plot" << gp.file1d(blocks_in) << "with lines title 'blocks_in'"<<std::endl;
	ifs.close();
	ifs.clear();

	std::vector<std::pair<float,float>> blocks_right;
	ifs.open(dirname+"no_tutor_v_0.000000_n_0.000000_blocks_right.ser");
	boost::archive::text_iarchive ia_right(ifs);
	ia_right & blocks_right;
	gp << "set xrange [0:5000]\nset yrange [0:5]\n";
	gp << "set terminal x11 4\n";
	gp << "plot" << gp.file1d(blocks_right) << "with lines title 'blocks_right'"<<std::endl;
	ifs.close();
	ifs.clear();

	std::vector<std::pair<float,float>> model_acc;
	ifs.open(dirname+"no_tutor_v_0.000000_n_0.000000_model_acc.ser");
	boost::archive::text_iarchive ia_model(ifs);
	ia_model & model_acc;
	gp << "set xrange [0:5000]\nset yrange [0:7]\n";
	gp << "set terminal x11 5\n";
	gp << "plot" << gp.file1d(model_acc) << "with lines title 'model_error'"<<std::endl;
	ifs.close();
	ifs.clear();

}


