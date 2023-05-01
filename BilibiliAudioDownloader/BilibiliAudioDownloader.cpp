//Cpp Library
#include <deque>
#include <string>
#include <io.h>
#include <fmt/core.h>
//Windows API
#include <direct.h>
#include <windows.h>
#include <time.h>
//Headers
#include <requests.hpp>
#include <utils.hpp>
#include <cli.hpp>
#include <infoList.hpp>
#include <fileSystem.hpp>
#include <stringHelper.hpp>
#include <Logger.hpp>

int main(int argc, char* argv[]) {
	if (argc <= 2) {
	    return 0;
	}
	system("@echo off");
	system("chcp>nul 2>nul 65001");

	std::string prefix = "./download/";
	if (!fileSystem::PathExists(prefix)) {
		fileSystem::CreateDirs(prefix);
	}

	unsigned long long startTime = utils::GetCurrentTimeStampMS();

	bool transcodingOpinion = false;
	bool BBDown = false;
	std::deque<requests::vedioInfo> infoList = cli::cli(argc, argv, transcodingOpinion, BBDown);

	if (transcodingOpinion) {
		std::string transcodingPath = "./transcoded/";
		if (!fileSystem::PathExists(transcodingPath)) {
			fileSystem::CreateDirs(transcodingPath);
		}
	}
	for (requests::vedioInfo eachInfo : infoList) {
		if (eachInfo.title == eachInfo.part) {
			fmt::print("{}({})\n", eachInfo.title, eachInfo.bvid);
		}
		else {
			fmt::print("{}.{}/{}({})\n", std::to_string(eachInfo.page), eachInfo.title, eachInfo.part, eachInfo.bvid);
		}
	}
	requests::getAudio(infoList, transcodingOpinion, BBDown);

	unsigned long long endTime = utils::GetCurrentTimeStampMS();
	std::string Duration = utils::round(std::to_string(endTime - startTime));

	fmt::print("Download Finish All! Time consuming: {} seconds", Duration);
}
