#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

#include <atomic>
#include <thread>
#include <mutex>

#include "CRC32.hpp"
#include "IO.hpp"

std::atomic_bool isFound{false};
std::vector<char> finalResult;
std::mutex consol_mtx;

/// @brief Переписывает последние 4 байта значением value
void replaceLastFourBytes(std::vector<char> &data, uint32_t value) {
  std::copy_n(reinterpret_cast<const char *>(&value), 4, data.end() - 4);
}

void work(size_t start, size_t end, std::vector<char> result, 
                  const uint32_t originalCrc32, std::atomic_bool& found_flag, 
                  std::vector<char>& final, size_t thread_id) {

  for (size_t i=start; i < end; i++) {
    // check the result from other threads
    if (found_flag.load()) {
      return;
    }
    // change last 4 bytes without giving access to the shared result
    replaceLastFourBytes(result, uint32_t(i));

    auto currentCrc32 = crc32(result.data(), result.size());

    // print progress for thread
    if (i % 10000000 == 0) {
      // lock mutex for normal print 
      consol_mtx.lock();

      std::cout << "progress for thread_id = " << thread_id
                << " is " << static_cast<double>(i - start) / static_cast<double>(end-start) * 100 << "%"
                << std::endl; 
      
      // unlock mutex
      consol_mtx.unlock();
    }

    if (currentCrc32 == originalCrc32) {
      // lock mutex
      consol_mtx.lock();

      // store result
      final = result;
      // change flag
      found_flag.store(true);
      std::cout << "Success\n";

      //ulock mutex
      consol_mtx.unlock();
      return;
    }
  }
}

/**
 * @brief Формирует новый вектор с тем же CRC32, добавляя в конец оригинального
 * строку injection и дополнительные 4 байта
 * @details При формировании нового вектора последние 4 байта не несут полезной
 * нагрузки и подбираются таким образом, чтобы CRC32 нового и оригинального
 * вектора совпадали
 * @param original оригинальный вектор
 * @param injection произвольная строка, которая будет добавлена после данных
 * оригинального вектора
 * @return новый вектор
 */
std::vector<char> hack(const std::vector<char> &original,
                       const std::string &injection) {
  const uint32_t originalCrc32 = crc32(original.data(), original.size());

  std::vector<char> result(original.size() + injection.size() + 4);
  auto it = std::copy(original.begin(), original.end(), result.begin());
  std::copy(injection.begin(), injection.end(), it);

  /*
   * Внимание: код ниже крайне не оптимален.
   * В качестве доп. задания устраните избыточные вычисления
   */
  const size_t maxVal = std::numeric_limits<uint32_t>::max();

  //////
  unsigned int num_threads = std::thread::hardware_concurrency();
  
  // calculate a block of the work
  size_t block_size = maxVal / num_threads;
  
  // vector of threads
  std::vector<std::thread> threads;
  
  for (size_t i = 0; i < num_threads; i++) {
    size_t start = i * block_size;
    size_t end = start + block_size;

    // carefull with i == last_thread
    if (i == num_threads - 1) {
      end = maxVal;
    }

    // add thread
    threads.push_back(std::thread(work, start, end, result, originalCrc32, std::ref(isFound), std::ref(finalResult), i));
  }

  for (auto it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  if (isFound.load())  {
    return finalResult;
  } else {
    throw std::logic_error("Can't hack");
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Call with two args: " << argv[0]
              << " <input file> <output file>\n";
    return 1;
  }

  try {
    const std::vector<char> data = readFromFile(argv[1]);
    const std::vector<char> badData = hack(data, "He-he-he");
    writeToFile(argv[2], badData);
  } catch (std::exception &ex) {
    std::cerr << ex.what() << '\n';
    return 2;
  }
  return 0;
}
