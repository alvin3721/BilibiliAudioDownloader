#pragma once
//C Library
#include <stdio.h>
#include <cstring>
//Cpp Library
#include <deque>
#include <string>
#include <vector>
#include <fmt/core.h>
//Headers
#include <requests.hpp>
#include <utils.hpp>


namespace infoList {
	std::deque<requests::vedioInfo> getInfoList(std::deque<std::string> bvList) {
		std::deque<std::deque<requests::vedioInfo>> resultList;
		for (std::string& eachBv : bvList) {
			if (utils::findStr(eachBv, "@") == std::string::npos) {
				resultList.push_back(requests::getVedioInfo(eachBv));
			}
			else {
				if (eachBv.size() == 13) {
					eachBv.pop_back();
					resultList.push_back(requests::getVedioInfo(eachBv));
				}
				if (eachBv.size() > 13) {
					std::vector<std::string> option = utils::split(eachBv, "@");
					std::string bv = option[0];
					std::string pageOption = option[1];
					if (utils::findStr(pageOption, "-") == std::string::npos) {
						resultList.push_back(requests::getVedioInfo(bv, std::stoi(pageOption), std::stoi(pageOption)));
					}
					else {
						if (pageOption[pageOption.size() - 1] == '-') {
							resultList.push_back(requests::getVedioInfo(bv, std::stoi(utils::replaceStr(pageOption, "-", ""))));
						}
						else {
							std::vector<std::string> pageOptions = utils::split(pageOption, "-");
							std::string startPage = pageOptions[0];
							std::string endPage = pageOptions[1];
							resultList.push_back(requests::getVedioInfo(bv, std::stoi(startPage), std::stoi(endPage)));
						}
					}
				}
			}
		}
		std::deque<requests::vedioInfo> result;
		for (std::deque<requests::vedioInfo> eachResult : resultList) {
			if (eachResult.size() == 1) {
				result.push_back(eachResult[0]);
			}
			else {
				for (requests::vedioInfo eachInfo : eachResult) {
					result.push_back(eachInfo);
				}
			}
		}
		return result;
	}

	std::deque<std::string> getBvList(int argc, char* argv[]) {
		std::deque<std::string> bvList;
		for (int i = 2; i < argc; i++) {
			bvList.push_back(argv[i]);
		}
		return bvList;
	}
	std::deque<std::string> getBvList(char* fileName) {
		std::ifstream infile(fileName);
		std::string line;
		std::deque<std::string> bvList;
		fmt::print("Opening file: {}\n", fileName);
		if (infile) {
			while (std::getline(infile, line)) {
				bvList.push_back(line);
			}
			return bvList;
		}
		else {
			fmt::print("ERROR: Failed to open file \"{}\"\n", fileName);
			exit(-1);
		}
	}
}
