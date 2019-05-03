num = 1
div = 8
amtCheck = 100000
totalCheck = []

while num < amtCheck:
    rem = num % div
    ans = float(rem * div)
    check = ans.is_integer()
    if check == True:
        totalCheck.append(check)
    else:
        totalCheck.append(check)
    num = num + 1

if False in totalCheck:
    print("There was a decimal")
else:
    print("The proof is a success!!")
