//
// Created by subingwen
//

#include "RsaCrypto.h"
#include "Base64.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <assert.h>

RsaCrypto::RsaCrypto(string fileName, KeyType type)
{
    BIO* bio = BIO_new_file(fileName.data(), "rb");
    assert(bio != NULL);
    if(type == PublicKey)
    {
        PEM_read_bio_PUBKEY(bio, &m_pubKey, NULL, NULL);
    }
    else
    {
        PEM_read_bio_PrivateKey(bio, &m_priKey, NULL, NULL);
    }
    BIO_free(bio);
}

RsaCrypto::~RsaCrypto()
{
    if(m_pubKey)
    {
        EVP_PKEY_free(m_pubKey);
    }
    if(m_priKey)
    {
        EVP_PKEY_free(m_priKey);
    }
}

void RsaCrypto::parseStringToKey(string data, KeyType type)
{
    BIO* bio = BIO_new_mem_buf(data.data(), data.size());
    assert(bio != NULL);
    if(type == PublicKey)
    {
        PEM_read_bio_PUBKEY(bio, &m_pubKey, NULL, NULL);
    }
    else
    {
        PEM_read_bio_PrivateKey(bio, &m_priKey, NULL, NULL);
    }
    BIO_free(bio);
}

void RsaCrypto::generateRsaKey(KeyLength bits, string pub, string pri)
{
    // 创建秘钥上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    // 初始化ctx
    int ret = EVP_PKEY_keygen_init(ctx);
    assert(ret == 1);
    // 指定密钥对长度
    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits);
    assert(ret == 1);

    // 生成密钥对
    ret = EVP_PKEY_generate(ctx, &m_priKey);
    assert(ret == 1);
    // 释放上下文
    EVP_PKEY_CTX_free(ctx);

    // 将秘钥写入到磁盘文件
    BIO* bio = BIO_new_file(pri.data(), "wb");
    ret = PEM_write_bio_PrivateKey(bio, m_priKey, NULL, NULL, 0, NULL, NULL);
    assert(ret == 1);
    BIO_flush(bio);
    BIO_free(bio);

    // 写公钥
    bio = BIO_new_file(pub.data(), "wb");
    ret = PEM_write_bio_PUBKEY(bio, m_priKey);
    assert(ret == 1);
    BIO_flush(bio);
    BIO_free(bio);
}

string RsaCrypto::pubKeyEncrypt(string data)
{
    // 创建上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new(m_pubKey, NULL);
    assert(ctx != NULL);
    // 设置加密和填充模式
    int ret = EVP_PKEY_encrypt_init(ctx);
    assert(ret == 1);

    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    assert(ret == 1);

    // 使用公钥加密
    size_t outlen = 0;
    ret = EVP_PKEY_encrypt(ctx, NULL, &outlen,
                           reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);
    unsigned char *out = new unsigned char[outlen];
    ret = EVP_PKEY_encrypt(ctx, out, &outlen,
                           reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);

    Base64 base;
    string retStr = base.encode(reinterpret_cast<char *>(out), outlen);
    // 释放资源
    delete[]out;
    EVP_PKEY_CTX_free(ctx);
    return retStr;
}

string RsaCrypto::priKeyDecrypt(string data)
{
    // 创建上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new(m_priKey, NULL);
    assert(ctx != NULL);
    // 设置加密和填充模式
    int ret = EVP_PKEY_decrypt_init(ctx);
    assert(ret == 1);

    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    assert(ret == 1);

    Base64 base;
    data = base.decode(data);
    // 使用私钥解密
    size_t outlen = 0;
    ret = EVP_PKEY_decrypt(ctx, NULL, &outlen,
                           reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);
    unsigned char *out = new unsigned char[outlen];
    ret = EVP_PKEY_decrypt(ctx, out, &outlen,
                           reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);

    string retStr(reinterpret_cast<char *>(out), outlen);
    // 释放资源
    delete[]out;
    EVP_PKEY_CTX_free(ctx);
    return retStr;
}

string RsaCrypto::sign(string data, HashType hash)
{
    // 计算哈希值
    Hash h(hash);
    h.addData(data);
    string md = h.result(Hash::Type::Binary);

    // 创建上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new(m_priKey, NULL);
    assert(ctx != NULL);
    // 签名初始化和设置填充模式
    int ret = EVP_PKEY_sign_init(ctx);
    assert(ret == 1);

    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);

    // 设置签名使用的哈希算法
    ret = EVP_PKEY_CTX_set_signature_md(ctx, HashMethods.at(hash)());
    assert(ret == 1);

    // 数据签名
    size_t outlen = 0;
    ret = EVP_PKEY_sign(ctx, NULL, &outlen,
                        reinterpret_cast<const unsigned char *>(md.data()), md.size());
    assert(ret == 1);
    unsigned char *out = new unsigned char[outlen];
    ret = EVP_PKEY_sign(ctx, out, &outlen,
                        reinterpret_cast<const unsigned char *>(md.data()), md.size());
    ERR_print_errors_fp(stderr);
    assert(ret == 1);

    Base64 base;
    string retStr = base.encode(reinterpret_cast<char *>(out), outlen);
    // 释放资源
    delete[]out;
    EVP_PKEY_CTX_free(ctx);
    return retStr;
}

bool RsaCrypto::verify(string sign, string data, HashType hash)
{
    Base64 base;
    sign = base.decode(sign);
    // 计算哈希值
      Hash h(hash);
    h.addData(data);
    string md = h.result(Hash::Type::Binary);

    // 创建上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new(m_pubKey, NULL);
    assert(ctx != NULL);
    // 签名校验初始化和设置填充模式
    int ret = EVP_PKEY_verify_init(ctx);
    assert(ret == 1);

    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);

    // 设置校验签名使用的哈希算法
    ret = EVP_PKEY_CTX_set_signature_md(ctx, HashMethods.at(hash)());
    assert(ret == 1);

    // 签名校验
    ret = EVP_PKEY_verify(ctx,
                          reinterpret_cast<const unsigned char *>(sign.data()), sign.size(),
                          reinterpret_cast<const unsigned char *>(md.data()), md.size());
    EVP_PKEY_CTX_free(ctx);
    if(ret == 1)
    {
        return true;
    }
    return false;
}
