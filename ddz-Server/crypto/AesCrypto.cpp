//
// Created by subingwen
//

#include "AesCrypto.h"
#include "Hash.h"
#include <assert.h>
#include <openssl/aes.h>
AesCrypto::AesCrypto(Algorithm algorithm, string key)
{
    switch(algorithm)
    {
        case AES_CBC_128:
        case AES_ECB_128:
        case AES_OFB_128:
        case AES_CFB_128:
        case AES_CTR_128:
            assert(key.size() == 16);
            break;
        case AES_CBC_192:
        case AES_ECB_192:
        case AES_OFB_192:
        case AES_CFB_192:
        case AES_CTR_192:
            assert(key.size() == 24);
            break;
        case AES_CBC_256:
        case AES_ECB_256:
        case AES_OFB_256:
        case AES_CFB_256:
        case AES_CTR_256:
            assert(key.size() == 32);
            break;
    }

    m_type = algorithm;
    m_key = key;
}

AesCrypto::~AesCrypto()
{

}

string AesCrypto::encrypt(string text)
{
    return aesCrypto(text, ENCRYPTO);
}

string AesCrypto::decrypt(string text)
{
    return aesCrypto(text, DECRYPTO);
}

string AesCrypto::aesCrypto(string text, CryptoType type)
{
    unsigned char ivec[AES_BLOCK_SIZE];
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    assert(ctx);
    generateIvec(ivec);
    int ret = EVP_CipherInit_ex(ctx, m_algorithms.at(m_type)(),
                                NULL, reinterpret_cast<unsigned char*>(m_key.data()),
                                ivec, type);
    assert(ret);

    // 准备存储数据的内存
    int length  = text.size() + 1;
    if(length % AES_BLOCK_SIZE)
    {
        length = (length / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }
    unsigned char *out = new unsigned char[length];
    int outLen = 0, totalLen = 0;
    ret = EVP_CipherUpdate(ctx, out, &outLen,
                           reinterpret_cast<const unsigned char *>(text.data()), text.size());
    totalLen += outLen;
    assert(ret);

    ret = EVP_CipherFinal_ex(ctx, out + outLen, &outLen);
    totalLen += outLen;
    assert(ret);

    string outtext(reinterpret_cast<char *>(out), totalLen);
    delete[]out;
    EVP_CIPHER_CTX_free(ctx);
    return outtext;
}

void AesCrypto::generateIvec(unsigned char *ivec)
{
    Hash hs(HashType::Md5);
    hs.addData(m_key);
    std::string res = hs.result(Hash::Type::Binary);
    for(int i=0; i<AES_BLOCK_SIZE; ++i)
    {
        ivec[i] = res.at(i);
    }
}
