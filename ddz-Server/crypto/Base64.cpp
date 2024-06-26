//
// Created by subingwen
//

#include "Base64.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

string Base64::encode(string data)
{
    return encode(data.data(), data.size());
}

string Base64::encode(const char *data, int length)
{
    // 创建bio对象
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    // 组织bio连
    BIO_push(b64, mem);
    // 数据编码
    BIO_write(b64, data, length);
    BIO_flush(b64);
    // 把编码之后的数据读出来
    BUF_MEM* ptr;
    BIO_get_mem_ptr(b64, &ptr);
    string str(ptr->data, ptr->length);
    BIO_free_all(b64);
    return str;
}

string Base64::decode(string data)
{
    return decode(data.data(), data.size());
}

string Base64::decode(const char *data, int length)
{
    // 创建bio对象
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    // 组织bio连
    BIO_push(b64, mem);
    // 将待解码数据写入到mem节点
    BIO_write(mem, data, length);

    // 解码
    char* buf = new char[length];
    int ret = BIO_read(b64, buf, length);
    string out(buf, ret);
    BIO_free_all(b64);
    delete[]buf;
    return out;
}
