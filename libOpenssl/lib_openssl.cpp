#include "lib_openssl.h"

#include <iostream>

namespace bm {
int make_RSA_key(const std::string& privatePath, const std::string& publicPath, int bits)
{
    RSA* rsa = RSA_new();
    BIGNUM* b1 = BN_new();
    int r = BN_set_word(b1, RSA_F4);
    if (!r)
    {
        RSA_free(rsa);
        BN_free(b1);
        return -1;
    }
    r = RSA_generate_key_ex(rsa, bits, b1, NULL);
    if (!r)
    {
        RSA_free(rsa);
        BN_free(b1);
        return -1;
    }

    if (!privatePath.empty())
    {
        BIO* bp_private = BIO_new_file(privatePath.c_str(), "w+");
        r = PEM_write_bio_RSAPrivateKey(bp_private, rsa, NULL, NULL, 0, NULL, NULL);
        if (!r)
        {
            BIO_free(bp_private);
            RSA_free(rsa);
            BN_free(b1);
            return -1;
        }
        else
            BIO_free(bp_private);
    }

    if (!publicPath.empty())
    {
        BIO* bp_public = BIO_new_file(publicPath.c_str(), "w+");
        r = PEM_write_bio_RSAPublicKey(bp_public, rsa);
        if (!r)
        {
            BIO_free(bp_public);
            RSA_free(rsa);
            BN_free(b1);
            return -1;
        }
        else
            BIO_free(bp_public);
    }

    return 0;
}

int encrypt_RSA(const std::string& from, std::string& to, const std::string& publicKeyPath, bool isPublic)
{
    char* encrypted_data = NULL;
    RSA* rsa_key = NULL;
    FILE* file = NULL;
    int srcLength = 0;
    if (fopen_s(&file, publicKeyPath.c_str(), "rb") != 0)
        return -1;

    if (!file)
        return -1;

    BIO* bp = BIO_new_file(publicKeyPath.c_str(), "rb");

    rsa_key = PEM_read_bio_RSAPublicKey(bp, NULL, NULL, NULL);
    if (rsa_key == NULL)
    {
        BIO_free(bp);
        return -1;
    }

    srcLength = (int)from.length();
    int rsaLen = RSA_size(rsa_key);
    encrypted_data = new char[rsaLen];
    if (!encrypted_data)
    {
        fclose(file);
        BIO_free(bp);
        return -1;
    }
    memset(encrypted_data, 0, rsaLen);
    int offset = 0;
    int dataLen = rsaLen;
    if (srcLength > rsaLen)
        dataLen = rsaLen - RSA_PKCS1_PADDING_SIZE;

    std::string tmp;
    do
    {
        if (srcLength > rsaLen)
        {
            std::string t = from.substr(offset, dataLen);
            int r = (isPublic ? RSA_public_encrypt : RSA_private_encrypt)(dataLen, (const unsigned char*)t.c_str(), (unsigned char*)encrypted_data, rsa_key, RSA_PKCS1_PADDING);
            if (r < 0)
            {
                delete[] encrypted_data;
                RSA_free(rsa_key);
                fclose(file);
                BIO_free(bp);
                return -1;
            }
            else
            {
                tmp += std::string(encrypted_data, r);

                memset(encrypted_data, 0, rsaLen);
                offset += dataLen;
            }

        }
        else
        {
            int r = (isPublic ? RSA_public_encrypt : RSA_private_encrypt)(dataLen, (const unsigned char*)from.substr(offset, dataLen).c_str(), (unsigned char*)encrypted_data, rsa_key, RSA_PKCS1_PADDING);
            if (r < 0)
            {
                delete[] encrypted_data;
                RSA_free(rsa_key);
                fclose(file);
                BIO_free(bp);
                return -1;
            }
            else
                tmp += std::string(encrypted_data, r);
        }
    } while (offset < srcLength);

    to = tmp;

    delete[] encrypted_data;
    RSA_free(rsa_key);
    fclose(file);
    BIO_free(bp);

    return 0;
}

int decrypt_RSA(const std::string& from, std::string& to, const std::string& privateKeyPath, bool isPublic)
{
    char* decrypted_data = NULL;
    RSA* rsa_key = NULL;
    FILE* file = NULL;

    if (fopen_s(&file, privateKeyPath.c_str(), "rb") != 0)
        return false;
    if (!file)
        return -1;

    BIO* bp = BIO_new_file(privateKeyPath.c_str(), "rb");
    if (!bp)
    {
        fclose(file);
        return -1;
    }

    rsa_key = PEM_read_bio_RSAPrivateKey(bp, NULL, NULL, NULL);
    if (rsa_key == NULL)
        return -1;

    int rsaLen = RSA_size(rsa_key);
    decrypted_data = new char[rsaLen];
    if (!decrypted_data)
        return -1;
    memset(decrypted_data, 0, rsaLen);
    int srcLength = (int)from.size();
    int offset = 0;
    std::string tmp;
    do
    {
        if (srcLength > rsaLen)
        {
            int r = (isPublic ? RSA_public_decrypt : RSA_private_decrypt)(rsaLen, (unsigned char*)from.substr(offset, rsaLen).c_str(), (unsigned char*)decrypted_data, rsa_key, RSA_PKCS1_PADDING);
            if (r <= 0)
                return -1;

            tmp += std::string(decrypted_data, r);
            offset += rsaLen;

            memset(decrypted_data, 0, rsaLen);
        }
        else
        {
            int r = (isPublic ? RSA_public_decrypt : RSA_private_decrypt)(rsaLen, (unsigned char*)from.c_str(), (unsigned char*)decrypted_data, rsa_key, RSA_NO_PADDING);
            if (!r)
                return -1;

            tmp += std::string(decrypted_data, r);
            offset += rsaLen;
        }
    } while (offset < srcLength);

    to = tmp;
    delete[] decrypted_data;
    RSA_free(rsa_key);
    fclose(file);
    BIO_free(bp);

    return 0;
}
int encrypt_base64(const std::string& from, std::string& to, bool newLine)
{
    BIO* bmem = NULL;
    BIO* b64 = NULL;
    BUF_MEM* bptr;

    b64 = BIO_new(BIO_f_base64());
    if (!newLine) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, from.c_str(), (int)from.length());
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);
    BIO_set_close(b64, BIO_NOCLOSE);

    char* buff = (char*)malloc(bptr->length + 1);
    if (!buff)
        return -1;

    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = 0;
    BIO_free_all(b64);

    to = std::string(buff, bptr->length);
    free(buff);

    return 0;
}
int decrypt_base64(const std::string& from, std::string& to, bool newLine)
{
    BIO* b64 = NULL;
    BIO* bmem = NULL;
    char* buff = (char*)malloc(from.length());
    if (!buff)
        return -1;

    memset(buff, 0, from.length());
    b64 = BIO_new(BIO_f_base64());
    if (!newLine) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new_mem_buf(from.c_str(), (int)from.length());
    bmem = BIO_push(b64, bmem);
    int r = BIO_read(bmem, buff, (int)from.length());
    BIO_free_all(bmem);

    to = std::string(buff, r);
    free(buff);

    return 0;
}
}