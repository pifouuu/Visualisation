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

#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "../include/exponential_smoothing.hpp"

#include <algorithm>
#include <functional>

template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(),
                   std::back_inserter(result), std::plus<T>());
    return result;
}

std::vector<float> expo_smooth(std::vector<float>& raw, float alpha){
	std::vector<float> smooth;
	smooth.push_back(raw[0]);
	for (int i=1;i<raw.size();i++){
		smooth.push_back(alpha*raw[i]+(1-alpha)*smooth[i-1]);
	}
	return smooth;
}

std::vector<float> avg_smooth(std::vector<float>& raw, int ws){
	std::vector<float> smooth;
	for (int j=0;j<ws;j++){
		smooth.push_back(raw[j]);
	}
	for (int i=ws;i<raw.size()-ws;i++){
		float val = 0.;
		for (int k=-ws;k<ws;k++){
			val += raw[i+k];
		}
		smooth.push_back(val/(2*ws));
	}
	for (int i=raw.size()-ws;i<raw.size();i++){
		smooth.push_back(raw[i]);
	}
	return smooth;
}

int main() {
	Gnuplot gp;
	std::vector<std::string> actions;
	actions.push_back(std::string("GO_TO_EYE"));
	actions.push_back(std::string("LOOK_RED_BOX"));
	actions.push_back(std::string("LOOK_BLUE_BOX"));
	actions.push_back(std::string("PICK"));
	//actions.push_back(std::string("PUT_DOWN"));
	actions.push_back(std::string("PUT_IN"));
	actions.push_back(std::string("LOOK_RED_BLOCK_0"));
	//actions.push_back(std::string("LOOK_RED_BLOCK_1"));
	//actions.push_back(std::string("LOOK_BLUE_BLOCK_0"));
	//actions.push_back(std::string("LOOK_BLUE_BLOCK_1"));
	//actions.push_back(std::string("LOOK_BLUE_BLOCK_2"));
	int NUMACTIONS = actions.size();

	float alpha = 0.5;
	int ws = 10;

	/*bool ACT_ACC = false;
	bool ACT_TRY = false;
	bool ACT_SUC = false;
	bool BLOCKS_IN = false;
	bool BLOCKS_RIGHT = false;
	bool MODEL_ACC = false;
	bool MODEL_ACC_R = false;
	bool ACCU_R = true;
	bool ACCU_TUTOR_R = false;*/

/*	fs::path basedir("../myTexplore/resultats/");
	  fs::directory_iterator end_iter;

	  typedef std::multimap<std::time_t, fs::path> result_set_t;
	  result_set_t dirnames;

	  if ( fs::exists(basedir) && fs::is_directory(basedir))
	  {
	    for( fs::directory_iterator dir_iter(basedir) ; dir_iter != end_iter ; ++dir_iter)
	    {
	        result_set.insert(result_set_t::value_type(fs::last_write_time(dir_iter->path()), *dir_iter));
	    }
	  }
	  if(fs::is_directory(basedir)) {
	    std::cout << basedir << " is a directory containing:\n";

	    for(auto& entry : boost::make_iterator_range(fs::directory_iterator(basedir), {})){
	//      std::cout << entry << "\n";
	      dirnames.insert(result_set_t::value_type(fs::last_write_time(entry), entry));
	    }
	  }
	  auto folder = dirnames.rbegin();
	  auto myEnd = std::next(result_set.rbegin(),);
	  while (folder != myEnd && folder != dirnames.rend()){
	    std::cout << folder->second << std::endl;
	    ++folder;
	  }*/



	std::string basedir = "../myTexplore/resultats_2/";
	//std::string basedir = "/home/pierre/Dropbox/resultats/";
	std::vector<std::string> dirnames;
	dirnames.push_back("06-04-2017_19-49-26_v_0_n_50_tb_0_pretrain_500_fR_100_nbR_1_nbB_0/");
	dirnames.push_back("06-04-2017_21-54-47_v_50_n_0_tb_0_pretrain_500_fR_100_nbR_1_nbB_0/");
	dirnames.push_back("07-04-2017_00-22-37_v_0_n_0_tb_50_pretrain_500_fR_100_nbR_1_nbB_0/");
//	dirnames.push_back("01-04-2017_00-41-20_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_00-46-07_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_00-51-31_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_00-57-17_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-02-32_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-07-52_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-12-06_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-16-42_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-22-29_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-26-29_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-30-45_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-37-02_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-42-09_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-48-37_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-52-17_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_01-56-40_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-01-31_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-06-00_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-10-47_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-14-36_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-18-46_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-23-39_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-29-13_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-34-42_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-40-05_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-44-59_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");
//	dirnames.push_back("01-04-2017_02-48-48_v_20_n_20_tb_20_pretrain_100_fR_100_nbR_0_nbB_1/");

	std::string name;
	std::ifstream ifs;
	int numdir = dirnames.size();

	for (int i=0; i<NUMACTIONS; i++){
		gp << "set xrange [-500:3000]\nset yrange [0:0.3]\n";
		gp << "set title '" << actions[i] << "'\n";
		gp << "set terminal x11 "<< i <<" \n";
		gp << "plot";
		for (auto dirname: dirnames){
			name = basedir+dirname+"model_acc_";
			std::vector<float> graph;
			std::vector<int> x_axis;

			ifs.open(name+actions[i]+".ser");
			boost::archive::text_iarchive graph_archive(ifs);
			graph_archive & graph;
			ifs.close();
			ifs.clear();

			ifs.open(basedir+dirname+"x_axis.ser");
			boost::archive::text_iarchive axis_archive(ifs);
			axis_archive & x_axis;
			ifs.close();
			ifs.clear();

			gp << gp.file1d(boost::make_tuple(x_axis,expo_smooth(graph,alpha))) << "with lines title '"<< dirname <<"',";
		}
		gp << std::endl;
	}

	gp << "set xrange [-500:3000]\nset yrange [0:0.3]\n";
	gp << "set title 'global model'\n";
	gp << "set terminal x11 "<< NUMACTIONS+4+dirnames.size() <<" \n";
	gp << "plot";
	for (auto dirname: dirnames){

		std::vector<int> x_axis;
		ifs.open(basedir+dirname+"x_axis.ser");
		boost::archive::text_iarchive axis_archive(ifs);
		axis_archive & x_axis;
		ifs.close();
		ifs.clear();

		std::vector<float> graph_tot(x_axis.size(),0.);

		name = basedir+dirname+"model_acc_";

		for (int i=0;i<NUMACTIONS;i++){
			std::vector<float> graph;
			ifs.open(name+actions[i]+".ser");
			boost::archive::text_iarchive graph_archive(ifs);
			graph_archive & graph;
			ifs.close();
			ifs.clear();
			graph_tot = graph_tot+graph;
		}

		for (int i=0;i<graph_tot.size();i++){
			graph_tot[i]/=NUMACTIONS;
		}
		gp << gp.file1d(boost::make_tuple(x_axis,expo_smooth(graph_tot,alpha))) << "with lines title '"<< dirname <<"',";
	}
	gp << std::endl;

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

	gp << "set xrange [-500:3000]\nset yrange [0:0.1]\n";
	gp << "set title 'Reward model error'\n";
	gp << "set terminal x11 "<< NUMACTIONS+1 <<" \n";
	gp << "plot";
	for (auto dirname: dirnames){
		name = basedir+dirname+"reward_model_acc.ser";
		std::vector<float> graph;
		std::vector<int> x_axis;

		ifs.open(name);
		boost::archive::text_iarchive ia(ifs);
		ia & graph;
		ifs.close();
		ifs.clear();

		ifs.open(basedir+dirname+"x_axis.ser");
		boost::archive::text_iarchive axis_archive(ifs);
		axis_archive & x_axis;
		ifs.close();
		ifs.clear();

		gp << gp.file1d(boost::make_tuple(x_axis,expo_smooth(graph,alpha))) << "with lines title '"<< dirname <<"',";
	}
	gp << std::endl;

	gp << "set xrange [-500:3000]\nset yrange [-1000:0]\n";
	gp << "set title 'Cumulative reward'\n";
	gp << "set terminal x11 "<< NUMACTIONS+2 <<" \n";
	gp << "plot";
	for (auto dirname: dirnames){

		name = basedir+dirname+"accumulated_reward.ser";
		std::vector<float> graph;
		std::vector<int> x_axis;

		ifs.open(name);
		boost::archive::text_iarchive ia(ifs);
		ia & graph;
		ifs.close();
		ifs.clear();

		ifs.open(basedir+dirname+"x_axis.ser");
		boost::archive::text_iarchive axis_archive(ifs);
		axis_archive & x_axis;
		ifs.close();
		ifs.clear();

		gp << gp.file1d(boost::make_tuple(x_axis,graph)) << "with lines title '"<< dirname <<"',";
	}
	gp << std::endl;

	gp << "set xrange [-500:3000]\nset yrange [0:150]\n";
	gp << "set title 'Cumulative tutor reward'\n";
	gp << "set terminal x11 "<< NUMACTIONS+3 <<" \n";
	gp << "plot";
	for (auto dirname: dirnames){
		name = basedir+dirname+"accu_tutor_rewards.ser";
		std::vector<float> graph;
		std::vector<int> x_axis;

		ifs.open(name);
		boost::archive::text_iarchive ia(ifs);
		ia & graph;
		ifs.close();
		ifs.clear();

		ifs.open(basedir+dirname+"x_axis.ser");
		boost::archive::text_iarchive axis_archive(ifs);
		axis_archive & x_axis;
		ifs.close();
		ifs.clear();

		//gp << gp.file1d(boost::make_tuple(x_axis,avg_smooth(graph,ws))) << "with lines title '"<< dirname <<"',";
		gp << gp.file1d(boost::make_tuple(x_axis,graph)) << "with lines title '"<< dirname <<"',";
	}
	gp << std::endl;

	gp << "set xrange [-500:3000]\nset yrange [0:1]\n";
	for (int i =0;i<dirnames.size();i++){
		gp << "set title 'Q values content : " << dirnames[i] << "'\n";
		gp << "set terminal x11 "<< NUMACTIONS+4+i <<" \n";
		std::vector<float> graph_reward;
		std::vector<float> graph_sync;
		std::vector<float> graph_var;
		std::vector<float> graph_nov;
		std::vector<int> x_axis;

		ifs.open(basedir+dirnames[i]+"reward_prop.ser");
		boost::archive::text_iarchive reward_archive(ifs);
		reward_archive & graph_reward;
		ifs.close();
		ifs.clear();

		ifs.open(basedir+dirnames[i]+"sync_prop.ser");
		boost::archive::text_iarchive sync_archive(ifs);
		sync_archive & graph_sync;
		ifs.close();
		ifs.clear();

		ifs.open(basedir+dirnames[i]+"var_prop.ser");
		boost::archive::text_iarchive var_archive(ifs);
		var_archive & graph_var;
		ifs.close();
		ifs.clear();

		ifs.open(basedir+dirnames[i]+"nov_prop.ser");
		boost::archive::text_iarchive nov_archive(ifs);
		nov_archive & graph_nov;
		ifs.close();
		ifs.clear();

		ifs.open(basedir+dirnames[i]+"x_axis.ser");
		boost::archive::text_iarchive axis_archive(ifs);
		axis_archive & x_axis;
		ifs.close();
		ifs.clear();




		gp << "plot '-' with lines title 'external reward', '-' with lines title 'sync bonus', "
				"'-' with lines title 'variance bonus', '-' with lines title 'novelty bonus'\n";
		gp.send1d(boost::make_tuple(x_axis,expo_smooth(graph_reward, alpha)));
		gp.send1d(boost::make_tuple(x_axis,expo_smooth(graph_sync, alpha)));
		gp.send1d(boost::make_tuple(x_axis,expo_smooth(graph_var, alpha)));
		gp.send1d(boost::make_tuple(x_axis,expo_smooth(graph_nov, alpha)));

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


