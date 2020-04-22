#ifndef __LIBOPEN_SSL_H__
#define __LIBOPEN_SSL_H__
#include <string>

#include "openssl/rsa.h"
#include "openssl/pem.h"

namespace bm {
int make_RSA_key(const std::string& privatePath, const std::string& publicPath, int bits);
int encrypt_RSA(const std::string& from, std::string& to, const std::string& keyPath, bool isPublic);
int decrypt_RSA(const std::string& from, std::string& to, const std::string& keyPath, bool isPublic);
int encrypt_base64(const std::string& from, std::string& to, bool newLine);
int decrypt_base64(const std::string& from, std::string& to, bool newLine);
}
#endif
