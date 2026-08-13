#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <cstdio>

#include "logger/Logger.h"

TEST(LoggerTest, WriteToFile) {
    const std::string fname = "test_write.log";
    std::remove(fname.c_str());          

    {
        logger::Logger logger(fname, logger::LogLevel::Info);
        ASSERT_TRUE(logger.isValid()) << "No log receiver " << logger.lastError();

        bool result = logger.log("Hello, world!", logger::LogLevel::Info);
        EXPECT_TRUE(result);
    } 

    std::ifstream file(fname);
    ASSERT_TRUE(file.is_open()) << "File was not created!";

    std::string line;
    std::getline(file, line);
    file.close();

    std::cout << "READ: [" << line << "]" << std::endl;

    EXPECT_FALSE(line.empty());
    EXPECT_NE(line.find("Hello, world!"), std::string::npos);

    std::remove(fname.c_str());
}

TEST(LoggerTest, LevelFiltering) {
    const std::string fname = "test_level.log";
    std::remove(fname.c_str());

    {
        logger::Logger logger(fname, logger::LogLevel::Error);
        ASSERT_TRUE(logger.isValid());

        EXPECT_FALSE(logger.log("Debug message", logger::LogLevel::Debug));
        EXPECT_FALSE(logger.log("Info message",  logger::LogLevel::Info));
        EXPECT_TRUE (logger.log("Error message", logger::LogLevel::Error));
    }

    std::ifstream file(fname);
    std::string line;
    std::getline(file, line);
    file.close();

    EXPECT_NE(line.find("Error"), std::string::npos);
    EXPECT_EQ(line.find("Debug"), std::string::npos);
    EXPECT_EQ(line.find("Info"),  std::string::npos);

    std::remove(fname.c_str());
}