#ifndef __LIBOPEN_SSL_H__
#define __LIBOPEN_SSL_H__
#include <string>

#include "openssl/rsa.h"
#include "openssl/pem.h"

namespace bm {
int makeRSAKey(const std::string& privatePath, const std::string& publicPath, int bits);
int encryptRSA(const std::string& from, std::string& to, const std::string& publicKeyPath);
int decryptRSA(const std::string& from, std::string& to, const std::string& privateKeyPath);
int encryptBase64(const std::string& from, std::string& to, bool newLine);
int decryptBase64(const std::string& from, std::string& to, bool newLine);
}
#endif
