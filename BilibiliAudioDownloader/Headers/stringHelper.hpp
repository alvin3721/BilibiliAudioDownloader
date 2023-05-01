#pragma once
//C Libaray
#include <stdio.h>
#include <cstring>
//Cpp Library
#include <string>
#include <io.h>
#include <vector>
//Windows API
#include <windows.h>
//Headers



namespace stringHelper {
	std::wstring str2wstr(const std::string& str, UINT codePage = CP_UTF8) {
		auto len = MultiByteToWideChar(codePage, 0, str.c_str(), -1, nullptr, 0);
		auto* buffer = new wchar_t[len];
		MultiByteToWideChar(codePage, 0, str.c_str(), -1, buffer, len);
		std::wstring result = std::wstring(buffer);
		delete[] buffer;
		return result;
	}

	std::string wstr2str(const std::wstring& wstr, UINT codePage = CP_UTF8) {
		auto len = WideCharToMultiByte(codePage, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		char* buffer = new char[len];
		WideCharToMultiByte(codePage, 0, wstr.c_str(), -1, buffer, len, nullptr, nullptr);
		std::string result = std::string(buffer);
		delete[] buffer;
		return result;
	}

	std::vector<std::string> SplitStrWithPattern(const std::string& str, const std::string& pattern) {
		std::vector<std::string> resVec;

		if (str.empty())
			return resVec;

		std::string strs = str + pattern;

		size_t pos = strs.find(pattern);
		size_t size = strs.size();

		while (pos != std::string::npos) {
			std::string x = strs.substr(0, pos);
			resVec.push_back(x);
			strs = strs.substr(pos + 1, size);
			pos = strs.find(pattern);
		}

		return resVec;
	}

	/*
	 * @brief Replace all founded sub string and modify input str
	 */
	std::string& ReplaceStr(std::string& str, const std::string& old_value, const std::string& new_value) {
		for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
			if ((pos = str.find(old_value, pos)) != std::string::npos)
				str.replace(pos, old_value.length(), new_value);
			else
				break;
		}
		return str;
	}
	std::wstring& wReplaceStr(std::wstring& str, const std::wstring& old_value, const std::wstring& new_value) {
		for (std::string::size_type pos(0); pos != std::wstring::npos; pos += new_value.length()) {
			if ((pos = str.find(old_value, pos)) != std::wstring::npos)
				str.replace(pos, old_value.length(), new_value);
			else
				break;
		}
		return str;
	}
	std::wstring& wReplaceStr(std::wstring& str, const std::string& old_value, const std::string& new_value) {
		std::wstring wold_value = stringHelper::str2wstr(old_value);
		std::wstring wnew_value = stringHelper::str2wstr(new_value);
		for (std::string::size_type pos(0); pos != std::wstring::npos; pos += wnew_value.length()) {
			if ((pos = str.find(wold_value, pos)) != std::wstring::npos)
				str.replace(pos, wold_value.length(), wnew_value);
			else
				break;
		}
		return str;
	}

	bool StartsWith(const std::string& str, const std::string& start) {
		size_t srcLen = str.size();
		size_t startLen = start.size();
		if (srcLen >= startLen) {
			std::string temp = str.substr(0, startLen);
			if (temp == start)
				return true;
		}

		return false;
	}

	bool EndsWith(const std::string& str, const std::string& end) {
		size_t srcLen = str.size();
		size_t endLen = end.size();
		if (srcLen >= endLen) {
			std::string temp = str.substr(srcLen - endLen, endLen);
			if (temp == end)
				return true;
		}

		return false;
	}
}