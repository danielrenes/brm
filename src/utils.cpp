#include "utils.h"

#include <chrono>
#include <fstream>
#include <string>
#include <sstream>

#include <openssl/md5.h>

namespace Brm
{
  std::string read_file(const std::string& file_path)
  {
    std::ifstream ifs(file_path);
    std::stringstream file_buffer;
    file_buffer << ifs.rdbuf();
    return file_buffer.str();
  }

  void copy_file(const std::string& src, const std::string& dst)
  {
    std::ifstream src_stream(src.c_str(), std::ios::binary);
    std::ofstream dst_stream(dst.c_str(), std::ios::binary);
    dst_stream << src_stream.rdbuf();
  }

  std::string md5_checksum(std::string content)
  {
    unsigned char digest[MD5_DIGEST_LENGTH];

    const char* c_content = content.c_str();

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, c_content, content.length());
    MD5_Final(digest, &ctx);

    std::stringstream hash_buffer;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
      hash_buffer << std::hex << (unsigned int) digest[i];
    }

    return hash_buffer.str();
  }

  int elapsed_minutes(std::chrono::system_clock::time_point from,
                      std::chrono::system_clock::time_point to) {
    auto duration_in_minutes = std::chrono::duration_cast<std::chrono::minutes>(to - from);
    return duration_in_minutes.count();
  }
}
