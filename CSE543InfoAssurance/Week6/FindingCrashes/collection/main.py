import itertools
import subprocess

perms = list(itertools.permutations(['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'], 4))
# write them as a static array in c
f = open("results.txt", "w")
for perm in perms:
    str_perm = ''.join(perm)
    # do str_perm | ./p_6 using subprocess
    # write the output to results.txt
    # echo "str_perm" | ./p_6
    r6 = subprocess.call(["echo", str_perm, "|", "./p_6"])
    r7 = subprocess.call(["echo", str_perm, "|", "./p_7"])
    # check if r6 or r7 are not int
    if r6 != 0 or r7 != 0:
        f.write(str_perm + "r6: " + str(r6) + " r7:" + str(r7))
    
    
f.close()