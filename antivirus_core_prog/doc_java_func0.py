#! /usr/bin/python2
import re

base_str = "11KTIZP1vCrV5482yeLexeuoyuSXJWQJ2dtxDzauCAfck4ICy0iM7RSEi"

def func0(input_str):
    res_str = ""
    if len(input_str) > 0:
        num = int(input_str[0:2],16)
        index = -1
        for i in xrange(2,len(input_str),2):
            next_num = int(input_str[i:i+2],16)
            if index < (len(base_str) - 1):
                index += 1
            else:
                index = 0
            res = next_num ^ ord(base_str[index])
            if res <= num:
                res = 255 + res - num
            else:
                res -= num
            if res == 34 or res == 92:
                res_str += "\\"
            res_str = res_str + str(unichr(res))
            num = next_num
        
    return res_str

if __name__ == '__main__':
    pat = r'sfunc0\(\"[a-zA-Z0-9]*\"\)'
    text = ""
    with open("DOCUMENTO_deobf.java",'r') as ob_file:
        text = ob_file.read()
    result = re.findall(pat, text)
    for s in result:
        text = text.replace(s, "\""+func0(s[8:-2])+"\"")
    with open("DOCUMENTO_res.java",'w') as deob_file:
        deob_file.write(text)
