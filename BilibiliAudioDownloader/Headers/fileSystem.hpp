#pragma once
#include <fmt/core.h>
#include <utils.hpp>
#include <fstream>
#include <iostream>
#include <stringHelper.hpp>


namespace fileSystem {
    enum class FileOpenMode : int {
        ReadMode,
        WriteMode,
        AppendMode
    };

	bool CreateDirs(const std::string path) {
		std::error_code ec;
		return std::filesystem::create_directories(std::filesystem::path(stringHelper::str2wstr(path)).remove_filename(), ec);
	}
    bool CreateDirs(const std::wstring path) {
        std::error_code ec;
        return std::filesystem::create_directories(std::filesystem::path(path).remove_filename(), ec);
    }

	bool PathExists(const std::string path) {
		return std::filesystem::exists(stringHelper::str2wstr(path));
	}
    bool PathExists(const std::wstring path) {
        return std::filesystem::exists(path);
    }

    std::fstream OpenFile(const std::string path, const int Mode, bool binaryOpinion = false) {
        try {
            CreateDirs(path);

            FileOpenMode fMode = (FileOpenMode)(Mode);
            std::ios_base::openmode mode = std::ios_base::in;
            if (fMode == FileOpenMode::WriteMode) {
                std::fstream tmp(stringHelper::str2wstr(path), std::ios_base::app);
                tmp.flush();
                tmp.close();
                mode |= std::ios_base::out;
            }
            else if (fMode == FileOpenMode::AppendMode)
                mode |= std::ios_base::app;

            bool isBinary = false;
            if (binaryOpinion) {
                isBinary = true;
                mode |= std::ios_base::binary;
            }

            std::fstream fs(stringHelper::str2wstr(path), mode);
            if (!fs.is_open()) {
                fmt::print("ERROR: Fail to open file\n{}\n", path);
            }
            return fs;
        }
        catch (const std::filesystem::filesystem_error& e) {
            fmt::print("ERROR: Fail to open file\n{}\n", path);
        }
        fmt::print("ERROR: Fail in open file\n");
    }
    std::ofstream OpenOutFile(const std::string path, const int Mode, bool binaryOpinion = false) {
        try {
            CreateDirs(path);

            FileOpenMode fMode = (FileOpenMode)(Mode);
            std::ios_base::openmode mode = std::ios_base::in;
            if (fMode == FileOpenMode::WriteMode) {
                std::fstream tmp(stringHelper::str2wstr(path), std::ios_base::app);
                tmp.flush();
                tmp.close();
                mode |= std::ios_base::out;
            }
            else if (fMode == FileOpenMode::AppendMode)
                mode |= std::ios_base::app;

            bool isBinary = false;
            if (binaryOpinion) {
                isBinary = true;
                mode |= std::ios_base::binary;
            }

            std::ofstream fs(stringHelper::str2wstr(path), mode);
            if (!fs.is_open()) {
                fmt::print("ERROR: Fail to open file\n{}\n", path);
            }
            return fs;
        }
        catch (const std::filesystem::filesystem_error& e) {
            fmt::print("ERROR: Fail to open file\n{}\n", path);
        }
        fmt::print("ERROR: Fail in open file\n");
    }
}