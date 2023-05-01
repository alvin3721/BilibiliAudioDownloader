#pragma once
//C Libaray
#include <stdio.h>
#include <cstring>
//Cpp Library
#include <deque>
#include <string>
#include <math.h>
#include <io.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <iostream>
//Windows API
#include <direct.h>
#include <time.h>
//Third_parts Library
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
//Headers
#include <utils.hpp>
#include <fileSystem.hpp>
#include <Logger.hpp>


namespace requests {
	struct vedioInfo {
		std::string author;
		std::string bvid;
		std::string title;
		int cid;
		std::string part;
		int page;
	};

	nlohmann::json getData(const std::string url) {
		cpr::Response res = cpr::Get(cpr::Url{ url });
		if (res.status_code >= 300) {
			fmt::print("HTTP ERROR: {}\n", res.status_code);
		}
		else {
			nlohmann::json jsonData = nlohmann::json::parse(res.text);
			if (jsonData["data"].is_null()) {
				fmt::print("ERROR: Data Not Found!\n");
				return nlohmann::json::value_t::null;
			}
			else {
				return jsonData["data"];
			}
		}
		return nlohmann::json::value_t::null;
	}
	
	std::deque<requests::vedioInfo> getVedioInfo(std::string bvid) {
		std::deque<requests::vedioInfo> infoList;
		std::string url = "https://api.bilibili.com/x/web-interface/view?bvid=" + bvid;
		nlohmann::json data = requests::getData(url);

		if (!data.is_null()) {
			unsigned int count = data["pages"].size();
			if (count == 1) {
				requests::vedioInfo info = { data["owner"]["name"], bvid, data["title"], data["pages"][0]["cid"], data["pages"][0]["part"], 1 };
				infoList.push_back(info);
				return infoList;
			}
			else {
				if (data["pages"][count - 1]["page"] > 1) {
					for (nlohmann::json eachPage : data["pages"]) {
						requests::vedioInfo info = { data["owner"]["name"], bvid, data["title"], eachPage["cid"], eachPage["part"], eachPage["page"] };
						infoList.push_back(info);
					}
					//for (unsigned int i = 1; i <= count; i++) {
					//	if (data["pages"][i]["page"] > 1) {
					//		requests::vedioInfo info = { data["owner"]["name"], bvid, data["title"], data["pages"][i - 1]["cid"], data["pages"][i - 1]["part"], data["pages"][i - 1]["page"] };
					//		infoList.push_back(info);
					//	}
					//}
					return infoList;
				}
				else {
					fmt::print("ERROR: The value of the start page is greater than the maximum value pulled!\n");
					return infoList;
				}
			}
		}
		else {
			("ERROR: Can't get Info of vedio {}!\n", bvid);
			return infoList;
		}
	}

	std::deque<requests::vedioInfo> getVedioInfo(std::string bvid, unsigned int startPage, unsigned int endPage = 0) {
		std::deque<requests::vedioInfo> infoList;
		std::string url = "https://api.bilibili.com/x/web-interface/view?bvid=" + bvid;
		nlohmann::json data = requests::getData(url);

		if (!data.is_null()) {
			unsigned int count = data["pages"].size();
			if (endPage > count) {
				fmt::print("WARN: The value of the end page exceeds the maximum value pulled!\n");
				("WARN: The value of the end page has been automatically replaced with the maximum value pulled!\n");
				endPage = count;
			}
			if (endPage == 0) {
				endPage = count;
			}
			if (endPage < startPage) {
				("WARN: The value of startPage exceeds the endPage!\n");
				return infoList;
			}
			if (count == 1) {
				requests::vedioInfo info = { data["owner"]["name"], bvid, data["title"], data["pages"][0]["cid"], data["pages"][0]["part"], 1 };
				infoList.push_back(info);
				return infoList;
			}
			else {
				if (startPage <= data["pages"][count - 1]["page"]) {
					for (unsigned int i = startPage; i <= endPage; i++) {
						if (data["pages"][i - 1]["page"] >= startPage) {
							requests::vedioInfo info = { data["owner"]["name"], bvid, data["title"], data["pages"][i - 1]["cid"], data["pages"][i - 1]["part"], data["pages"][i - 1]["page"] };
							infoList.push_back(info);
						}
					}
					return infoList;
				}
				else {
					fmt::print("ERROR: The value of the start page is greater than the maximum value pulled!\n");
					return infoList;
				}
			}
		}
		else {
			fmt::print("ERROR: Can't get Info of vedio {}!\n", bvid);
			return infoList;
		}
	}

	nlohmann::json getPages(std::string bvid) {
		nlohmann::json data = requests::getData("https://api.bilibili.com/x/web-interface/view?bvid=" + bvid);
		if (!data.is_null()) {
			return data["pages"];
		}
		else {
			fmt::print("ERROR: Empty data!\n");
		}
		return nlohmann::json::value_t::null;
	}
	void getAudio(std::deque<requests::vedioInfo> infoList, bool transcodingOpinion = false, bool BBDown = false) {
		for (requests::vedioInfo& vedioInfo : infoList) {
			unsigned long long startTime = utils::GetCurrentTimeStampMS();
			if (BBDown) {
				std::string cmd = "BBDown.exe " + vedioInfo.bvid + " --audio-only -p " + std::to_string(vedioInfo.page);
				system(utils::UTF8ToGB(cmd.c_str()).c_str());
				continue;
			}
			std::string baseURL = "http://api.bilibili.com/x/player/playurl?fnval=16&";
			std::string URL = baseURL + "bvid=" + vedioInfo.bvid + "&cid=" + std::to_string(vedioInfo.cid);
			nlohmann::json data = requests::getData(URL);
			std::string audioURL = data["dash"]["audio"][0]["baseUrl"];

			std::string title = utils::strRepair(vedioInfo.title);
			std::string part = utils::strRepair(vedioInfo.part);


			//std::ofstream of;
			//std::string uFileName = "./download/" + vedioInfo.cid + '-' + vedioInfo.page + '.mp3';
			std::string fileName;
			if (part == title) {
				fmt::print("Downloading: {}.mp3 ", title);
				fileName = "./download/" + title + ".mp3";
				//of.open(utils::UTF8ToUnicode(fileName), std::ios::binary);
			}
			else {
				fmt::print("Downloading: {}/{}.{}.mp3 ", title, vedioInfo.page, part);
				std::string prefix = "./download/" + title + '/';
				if (!fileSystem::PathExists(utils::UTF8ToUnicode(prefix))) {
					fileSystem::CreateDirs(utils::UTF8ToUnicode(prefix));
				}
				fileName = "./download/" + title + '/' + std::to_string(vedioInfo.page) + '.' + part + ".mp3";
				//of.open(utils::UTF8ToUnicode(fileName), std::ios::binary);
			}
			std::ofstream of = fileSystem::OpenOutFile(utils::UnicodeToUTF8(stringHelper::str2wstr(fileName)), 1, true);
			cpr::Response res = cpr::Download(of,
				cpr::Url{ audioURL },
				cpr::Header{
					{"User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.13; rv:56.0) Gecko/20100101 Firefox/56.0"},
					{"Accept", "*/*"},
					{"Accept-Language", "en-US,en;q=0.5"},
					{"Accept-Encoding", "gzip, deflate, br"},
					{"Range", "bytes=0-"},
					{"Referer", "https://api.bilibili.com/x/web-interface/view?bvid=" + vedioInfo.bvid},
					{"Origin", "https://www.bilibili.com"},
					{"Connection", "keep-alive"}
				});
			of.close();
			of.flush();
			of.clear();

			unsigned long long endTime = utils::GetCurrentTimeStampMS();
			std::string Duration = utils::round(std::to_string(endTime - startTime));
			fmt::print("Finished({}s)\n", Duration);

			if (transcodingOpinion) {
				std::string originalPath;
				std::string outPutPath;

				if (title == part) {
					originalPath = "./download/" + title + ".mp3";
					outPutPath = "./transcoded/" + title + ".mp3";
				}
				else {
					originalPath = "./download/" + title + '/' + std::to_string(vedioInfo.page) + '.' + part + ".mp3";
					outPutPath = "./transcoded/" + title + '/' + std::to_string(vedioInfo.page) + '.' + part + ".mp3";
					std::string prefix = "./transcoded/" + title + '/';
					if (!fileSystem::PathExists(prefix)) {
						fileSystem::CreateDirs(prefix);
					}
				}
				//std::string program = "ffmpeg.exe -i \"" + originalPath + "\" -acodec libmp3lame -ab 192k -ac 2 -y -metadata Artist=\"" + vedioInfo.author + "\" \"" + outPutPath + "\"";
				//fmt::print("{}\n", program);
				//utils::noWindowStart(program, "", "log.txt");
				//std::string cmd = "NoWindowStartup.exe -p \"ffmpeg -i \\\"" + originalPath + "\\\" -acodec libmp3lame -ab 192k -ac 2 -y -metadata Artist=\\\"" + vedioInfo.author + "\\\" \\\"" + outPutPath + "\\\"\"";
				
				std::string cmd = "ffmpeg.exe -i \"" + originalPath + "\" -acodec libmp3lame -ab 192k -ac 2 -y -metadata Artist=\"" + vedioInfo.author + "\" \"" + outPutPath + "\"";
				system(utils::UTF8ToGB(cmd.c_str()).c_str());
			}
			Sleep(1);
		}
	}
}