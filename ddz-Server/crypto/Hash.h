//
// Created by subingwen
//

#ifndef SERVER_DDZ_HASH_H
#define SERVER_DDZ_HASH_H
#include <string>
#include <map>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
using namespace std;

enum class HashType:char{
    Md5,
    Sha1,
    Sha224,
    Sha256,
    Sha384,
    Sha512,
    Sha3_224,
    Sha3_256,
    Sha3_384,
    Sha3_512,
};
using hashFunc = const EVP_MD *(*)(void);
const map<HashType, hashFunc> HashMethods = {
        {HashType::Md5, EVP_md5},
        {HashType::Sha1, EVP_sha1},
        {HashType::Sha224, EVP_sha224},
        {HashType::Sha256, EVP_sha256},
        {HashType::Sha384, EVP_sha384},
        {HashType::Sha512, EVP_sha512},
        {HashType::Sha3_224, EVP_sha3_224},
        {HashType::Sha3_256, EVP_sha3_256},
        {HashType::Sha3_384, EVP_sha3_384},
        {HashType::Sha3_512, EVP_sha3_512}
};

const map<HashType, int> HashLengths = {
        {HashType::Md5, MD5_DIGEST_LENGTH},
        {HashType::Sha1, SHA_DIGEST_LENGTH},
        {HashType::Sha224, SHA224_DIGEST_LENGTH},
        {HashType::Sha256, SHA256_DIGEST_LENGTH},
        {HashType::Sha384, SHA384_DIGEST_LENGTH},
        {HashType::Sha512, SHA512_DIGEST_LENGTH},
        {HashType::Sha3_224, SHA224_DIGEST_LENGTH},
        {HashType::Sha3_256, SHA256_DIGEST_LENGTH},
        {HashType::Sha3_384, SHA384_DIGEST_LENGTH},
        {HashType::Sha3_512, SHA512_DIGEST_LENGTH}
};

class Hash
{
public:
    enum class Type:char{Binary, Hex};
    Hash(HashType type);
    ~Hash();

    // 添加数据
    void addData(string data);
    void addData(const char* data, int length);
    // 计算结果
    string result(Type type = Type::Hex);
private:
    EVP_MD_CTX *m_ctx = NULL;
    HashType m_type;
};


#endif //SERVER_DDZ_HASH_H
