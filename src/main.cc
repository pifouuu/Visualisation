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
#include <boost/serialization/list.hpp>
#include <boost/serialization/utility.hpp>
#include "../include/gnuplot-iostream.h"

#define NUMACTIONS 6

int main() {
	Gnuplot gp;
	std::vector<std::string> actions;
	actions.push_back(std::string("GO_TO_EYE"));
	actions.push_back(std::string("LOOK_RED_BOX"));
	actions.push_back(std::string("LOOK_BLUE_BOX"));
	actions.push_back(std::string("PICK"));
	//actions.push_back(std::string("PUT_DOWN"));
	actions.push_back(std::string("PUT_IN"));
	//actions.push_back(std::string("LOOK_RED_BLOCK_0"));
	//actions.push_back(std::string("LOOK_RED_BLOCK_1"));
	actions.push_back(std::string("LOOK_BLUE_BLOCK_0"));
	//actions.push_back(std::string("LOOK_BLUE_BLOCK_1"));
	//actions.push_back(std::string("LOOK_BLUE_BLOCK_2"));

	bool ACT_ACC = true;
	bool ACT_TRY = false;
	bool ACT_SUC = false;
	bool BLOCKS_IN = false;
	bool BLOCKS_RIGHT = false;
	bool MODEL_ACC = false;
	bool MODEL_ACC_R = true;
	bool ACCU_R = true;
	bool ACCU_TUTOR_R = false;

	std::string basedir = "../myTexplore/texplore_s_dep_tutor_14_20_2_";
	std::list<std::string> dirnames = {"n_10.000000_tb_0.000000_pretrain_0_"};
	dirnames.push_back("v_5.000000_n_5.000000_tb_10.000000_pretrain_2000_");
	dirnames.push_back("v_5.000000_n_5.000000_tb_10.000000_pretrain_0_");
	dirnames.push_back("v_50.000000_tb_0.000000_pretrain_0_");
	dirnames.push_back("v_10.000000_tb_0.000000_pretrain_0_");
	dirnames.push_back("v_10.000000_n_10.000000_tb_10.000000_pretrain_0_");
	dirnames.push_back("tb_50.000000_pretrain_0_");
	dirnames.push_back("tb_10.000000_pretrain_0_");
	dirnames.push_back("n_50.000000_tb_0.000000_pretrain_0_");
	dirnames.push_back("n_2.000000_tb_20.000000_pretrain_2000_");
	dirnames.push_back("n_10.000000_tb_0.000000_pretrain_0_");
	dirnames.push_back("n_10.000000_tb_0.000000_pretrain_0_");

	std::string name;
	std::ifstream ifs;
	int numdir = dirnames.size();

	for (int i=0; i<NUMACTIONS; i++){
		gp << "set xrange [0:15000]\nset yrange [0:0.5]\n";
		gp << "set title '" << actions[i] << "'\n";
		gp << "set terminal x11 "<< i <<" \n";
		gp << "plot";
		for (auto dirname: dirnames){
			name = basedir+dirname+"model_acc_";
			std::vector<std::pair<float,float>> graph;
			ifs.open(name+actions[i]+".ser");
			boost::archive::text_iarchive ia(ifs);
			ia & graph;
			ifs.close();
			ifs.clear();
			gp << gp.file1d(graph) << "with lines title '"<< dirname <<"',";
		}
		gp << std::endl;
	}

/*	if (ACT_TRY){
		std::vector<std::vector<std::pair<float,float>>> act_try(NUMACTIONS);


		std::string name = dirname+"act_try_";
		for (int i=0; i<NUMACTIONS; i++){
			ifs.open(name+actions[i]+".ser");
			boost::archive::text_iarchive ia(ifs);
			ia & act_try[i];
			ifs.close();
			ifs.clear();
		}

		gp << "set xrange [0:1000]\nset yrange [0:6000]\n";
		// Data will be sent via a temporary file.  These are erased when you call
		// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
		// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
		// and won't be deleted (this is useful when creating a script).
		gp << "plot";
		for (int i=0; i<NUMACTIONS; i++){
			gp << gp.file1d(act_try[i]) << "with lines title 'act_try_" << actions[i] <<"',";
		}
		gp << std::endl;
	}*/



	/*if (ACT_SUC){
		std::vector<std::vector<std::pair<float,float>>> act_suc(NUMACTIONS);
		name = dirname+"act_succes_";
		for (int i=0; i<NUMACTIONS; i++){
			ifs.open(name+actions[i]+".ser");
			boost::archive::text_iarchive ia(ifs);
			ia & act_suc[i];
			ifs.close();
			ifs.clear();
		}

		gp << "set xrange [0:3000]\nset yrange [0:300]\n";
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
	}
*/
/*	if (BLOCKS_IN){
		std::vector<std::pair<float,float>> blocks_in;
		ifs.open(dirname+"blocks_in.ser");
		boost::archive::text_iarchive ia_in(ifs);
		ia_in & blocks_in;
		gp << "set xrange [0:50000]\nset yrange [0:6]\n";
		gp << "set terminal x11 3\n";
		gp << "plot" << gp.file1d(blocks_in) << "with lines title 'blocks_in'"<<std::endl;
		ifs.close();
		ifs.clear();
	}

	if (BLOCKS_RIGHT){
		std::vector<std::pair<float,float>> blocks_right;
		ifs.open(dirname+"blocks_right.ser");
		boost::archive::text_iarchive ia_right(ifs);
		ia_right & blocks_right;
		gp << "set xrange [0:40000]\nset yrange [0:5]\n";
		gp << "set terminal x11 4\n";
		gp << "plot" << gp.file1d(blocks_right) << "with lines title 'blocks_right'"<<std::endl;
		ifs.close();
		ifs.clear();
	}*/

/*	if (MODEL_ACC) {
		std::vector<std::pair<int,float>> model_acc_1;
		std::vector<std::pair<int,float>> model_acc_2;
		std::vector<std::pair<int,float>> model_acc_3;
//		ifs.open(dirname1+"model_acc_test_only.ser");
//		boost::archive::text_iarchive ia_model_1(ifs);
//		ia_model_1 & model_acc_1;
		gp << "set xrange [0:3000]\nset yrange [0:0.2]\n";
		gp << "set terminal x11 5\n";
		gp << "plot";
//		gp << gp.file1d(model_acc_1) << "with lines title 'model error 1',";
//		ifs.close();
//		ifs.clear();
//		ifs.open(dirname2+"model_acc_test_only.ser");
//		boost::archive::text_iarchive ia_model_2(ifs);
//		ia_model_2 & model_acc_2;
//		gp << gp.file1d(model_acc_2) << "with lines title 'model error 2',";
//		ifs.close();
//		ifs.clear();
		ifs.open(dirname+"model_acc_test.ser");
		boost::archive::text_iarchive ia_model_3(ifs);
		ia_model_3 & model_acc_3;
		gp << gp.file1d(model_acc_3) << "with lines title 'model error 3'"<<std::endl;
		ifs.close();
		ifs.clear();
	}*/

	gp << "set xrange [0:15000]\nset yrange [0:20]\n";
	gp << "set title 'Reward model error'\n";
	gp << "set terminal x11 "<< NUMACTIONS <<" \n";
	gp << "plot";
	for (auto dirname: dirnames){
		name = basedir+dirname+"model_acc_test_r.ser";
		std::vector<std::pair<float,float>> graph;
		ifs.open(name);
		boost::archive::text_iarchive ia(ifs);
		ia & graph;
		ifs.close();
		ifs.clear();
		gp << gp.file1d(graph) << "with lines title '"<< dirname <<"',";
	}
	gp << std::endl;

	gp << "set xrange [0:15000]\nset yrange [0:2500]\n";
	gp << "set title 'Cumulative reward'\n";
	gp << "set terminal x11 "<< NUMACTIONS+1 <<" \n";
	gp << "plot";
	for (auto dirname: dirnames){
		name = basedir+dirname+"accumulated_rewards.ser";
		std::vector<std::pair<float,float>> graph;
		ifs.open(name);
		boost::archive::text_iarchive ia(ifs);
		ia & graph;
		ifs.close();
		ifs.clear();
		gp << gp.file1d(graph) << "with lines title '"<< dirname <<"',";
	}
	gp << std::endl;

/*	if (ACCU_TUTOR_R) {
		std::list<std::pair<float,float>> accu_tutor_r;
		ifs.open(dirname+"accu_tutor_rewards.ser");
		boost::archive::text_iarchive ia_r(ifs);
		ia_r & accu_tutor_r;
		gp << "set xrange [0:40000]\nset yrange [0:5000]\n";
		gp << "set terminal x11 7\n";
		gp << "plot" << gp.file1d(accu_tutor_r) << "with lines title 'accumultaed reward'"<<std::endl;
		ifs.close();
		ifs.clear();
	}*/


}


