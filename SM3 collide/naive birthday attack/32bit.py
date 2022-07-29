from gmssl import sm3, func
import time
A=dict()
start=time.time()
for i in range(2**16):
    str_i = bytes(str(i), encoding='utf-8')
    result = sm3.sm3_hash(func.bytes_to_list(str_i))
    A[result[0:8]]=i
print(A)
print(time.time()-start)
i=2**16+1
start=time.time()
while True:
    str_i = bytes(str(i), encoding='utf-8')
    num = sm3.sm3_hash(func.bytes_to_list(str_i))
    if A.get(num[0:8]) is not None:
        print("find collide:",A.get(num[0:8]),i)
        break
    i+=1
print(time.time()-start)
