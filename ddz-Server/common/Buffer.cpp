#include "Buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Log.h"
Buffer::Buffer(int size) :m_capacity(size)
{
	m_data = (char*)malloc(size);
	bzero(m_data, size);
}

Buffer::~Buffer()
{
	if (m_data != nullptr)
	{
		free(m_data);
	}
}

void Buffer::extendRoom(int size)//扩容 
{
	if (writeableSize() >= size)
	{
		return;
	}
	else if (m_readPos + writeableSize() >= size)
	{
		int readable = readableSize();
		// 移动内存
		memcpy(m_data, m_data + m_readPos, readable);
		// 更新位置
		m_readPos = 0;
		m_writePos = readable;
	}
	else
	{
		void* temp = realloc(m_data, m_capacity + size);
		if (temp == NULL)
		{
			return; // 失败了
		}
		memset((char*)temp + m_capacity, 0, size);
		// 更新数据
		m_data = static_cast<char*>(temp);
		m_capacity += size;
	}
}



int Buffer::appendString(const char* data, int size)
{
	if (data == nullptr || size <= 0)
	{
		return -1;
	}
	// 扩容
	extendRoom(size);
	// 数据拷贝
	memcpy(m_data + m_writePos, data, size);
	m_writePos += size;
	return 0;
}

int Buffer::appendString(const char* data)
{
	int size = strlen(data);
	int ret = appendString(data, size);
	return ret;
}

int Buffer::appendString(const string data)
{
	int ret = appendString(data.c_str(),data.size());
	return ret;
}

int Buffer::socketRead(int fd)
{
	struct iovec vec[2];
	int writeable = writeableSize();
	vec[0].iov_base = m_data + m_writePos;
	vec[0].iov_len = writeable;
	char* tmpbuf = (char*)malloc(40960);
	vec[1].iov_base = tmpbuf;
	vec[1].iov_len = 40960;
	int result = readv(fd, vec, 2);
	if (result == -1)
	{
		return -1;
	}
	else if (result <= writeable)
	{
		m_writePos += result;
	}
	else
	{
		m_writePos = m_capacity;
		appendString(tmpbuf, result - writeable);
	}
	free(tmpbuf);
	return result;
}

char* Buffer::findCRLF()
{
	char* ptr = (char*)memmem(m_data + m_readPos, readableSize(), "\r\n", 2);
	return ptr;
}

int Buffer::sendData(int socket)
{
	int readable = readableSize();
	if (readable > 0)
	{
		int count = send(socket, m_data + m_readPos, readable, MSG_NOSIGNAL);
		if (count > 0)
		{
			m_readPos += count;
			usleep(1);
		}
		return count;
	}
	return 0;
}
int Buffer::appendHead(const int length)
{
    Debug("小端数据长度%d",length);
    int len = htonl(length);
    Debug("大端数据长度%d",len);
    string head(reinterpret_cast<char*>(&len),sizeof(int));
    appendString(head);
    return 0;
}
int Buffer::appendPackage(const string data)
{
    appendHead(data.size());
    appendString(data);
    return 0;
}


