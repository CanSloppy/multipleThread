#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <thread>
#include <algorithm>
#include <atomic>

void processLine(const std::string& line,std::vector<int>& output)
{
    std::istringstream iss(line);
    std::string token;

    while (iss >> token)
    {
        try
        {
            int value = std::stoi(token);
            output.push_back(value);
        }
        catch (...)
        {
            std::cout << "error type";
        }
    }

    std::sort(output.begin(), output.end());
}

int main()
{
    std::ifstream inFile("C:/Users/Administrator/Desktop/input.txt");
    if (!inFile.is_open())
    {
        std::cerr << "Failed to open input.txt\n";
        return -1;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(inFile, line))
    {
        lines.push_back(line);
    }
    inFile.close();

    size_t lineCount = lines.size();
    std::vector<std::vector<int>> results(lineCount);

    // 线程数量：CPU 核心数
    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0)
        threadCount = 4;

    std::atomic<size_t> index{ 0 };
    std::vector<std::thread> threads;

    auto worker = [&]() {
        while (true)
        {
            size_t i = index.fetch_add(1);
            if (i >= lineCount)
                break;

            if (!lines[i].empty())
            {
                processLine(lines[i], results[i]);
            }
        }
        };

    for (unsigned int i = 0; i < threadCount; ++i)
    {
        threads.emplace_back(worker);
    }

    for (auto& t : threads)
    {
        if (t.joinable())
            t.join();
    }

    std::ofstream outFile("C:/Users/Administrator/Desktop/output.txt");
    if (!outFile.is_open())
    {
        std::cerr << "Failed to open output.txt\n";
        return -1;
    }

    for (const auto& vec : results)
    {
        for (size_t i = 0; i < vec.size(); ++i)
        {
            outFile << vec[i];
            if (i + 1 < vec.size())
                outFile << " ";
        }
        outFile << "\n";
    }

    outFile.close();
    return 0;
}
