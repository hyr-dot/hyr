import mysm3
import struct
from gmssl import sm3, func


M = str(1111111111111111) # 初始消息
M_hash = sm3.sm3_hash(func.bytes_to_list(bytes(M, encoding='utf-8')))
append = "1234567890"   # 附加消息
print("M: ",M)
print("M hash:" , M_hash)


pad_str = ""
pad = [] # 用于填充


def entend(M_hash, M_len, append):
    """
    SM3 length extend attack
    """
    vectors = []
    message = ""
    
    for r in range(0, len(M_hash), 8):
        vectors.append(int(M_hash[r:r + 8], 16))
    # 使用字母a进行消息填充
    if M_len > 64:
        for i in range(0, int(M_len / 64) * 64):
            message += 'a'
    for i in range(0, M_len % 64):
        message += 'a'
    message = func.bytes_to_list(bytes(message, encoding='utf-8'))
    message = padding(message)
    message.extend(func.bytes_to_list(bytes(append, encoding='utf-8')))
    return mysm3.sm3_hash(message, vectors)


def padding(pad_msg):
    global pad_str
    mlen = len(pad_msg)
    pad_msg.append(0x80)
    mlen += 1
    tail = mlen % 64
    range_end = 56
    if tail > range_end:
        range_end = range_end + 64
    for i in range(tail, range_end):
        pad_msg.append(0x00)
    bit_len = (mlen - 1) * 8
    pad_msg.extend([int(x) for x in struct.pack('>q', bit_len)])
    for j in range(int((mlen - 1) / 64) * 64 + (mlen - 1) % 64, len(pad_msg)):
        pad.append(pad_msg[j])
        pad_str += str(hex(pad_msg[j]))
    return pad_msg


extend_hash = entend(M_hash, len(M), append)
new_pad_msg = func.bytes_to_list(bytes(M, encoding='utf-8'))
new_pad_msg.extend(pad)
new_pad_msg.extend(func.bytes_to_list(bytes(append, encoding='utf-8')))
new_pad_msg_str = M + pad_str + append

new_hash = sm3.sm3_hash(new_pad_msg)

print("extend hash:" + extend_hash)
print("new hash:" + new_hash)
if new_hash == extend_hash:
    print("success")
else:
    print("fail")
