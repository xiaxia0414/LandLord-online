#ifndef AESCRYPTO_H
#define AESCRYPTO_H

#include <QObject>
#include <QByteArray>
#include <QMap>
#include <openssl/evp.h>

class AesCrypto : public QObject
{
    Q_OBJECT
public:
    enum Algorithm{
        // 16byte
        AES_ECB_128,
        AES_CBC_128,
        AES_CFB_128,
        AES_OFB_128,
        AES_CTR_128,
        // 24byte
        AES_ECB_192,
        AES_CBC_192,
        AES_CFB_192,
        AES_OFB_192,
        AES_CTR_192,
        // 32byte
        AES_ECB_256,
        AES_CBC_256,
        AES_CFB_256,
        AES_OFB_256,
        AES_CTR_256
    };
    enum CryptoType{DECRYPTO, ENCRYPTO};
    using algorithmFunc = const EVP_CIPHER *(*)(void);


    AesCrypto(Algorithm algorithm, QByteArray key, QObject* parent = nullptr);
    ~AesCrypto();

    // 加密
    QByteArray encrypt(QByteArray text);
    // 解密
    QByteArray decrypt(QByteArray text);
private:
    QByteArray aesCrypto(QByteArray text, CryptoType type);
    void generateIvec(unsigned char* ivec);
    const QMap<Algorithm, algorithmFunc> m_algorithms = {
        {AES_ECB_128, EVP_aes_128_ecb},
        {AES_CBC_128, EVP_aes_128_cbc},
        {AES_CFB_128, EVP_aes_128_cfb128},
        {AES_OFB_128, EVP_aes_128_ofb},
        {AES_CTR_128, EVP_aes_128_ctr},
        {AES_ECB_192, EVP_aes_192_ecb},
        {AES_CBC_192, EVP_aes_192_cbc},
        {AES_CFB_192, EVP_aes_192_cfb128},
        {AES_OFB_192, EVP_aes_192_ofb},
        {AES_CTR_192, EVP_aes_192_ctr},
        {AES_ECB_256, EVP_aes_256_ecb},
        {AES_CBC_256, EVP_aes_256_cbc},
        {AES_CFB_256, EVP_aes_256_cfb128},
        {AES_OFB_256, EVP_aes_256_ofb},
        {AES_CTR_256, EVP_aes_256_ctr}
    };
private:
    Algorithm m_type;
    QByteArray m_key;
signals:
};

#endif // AESCRYPTO_H
