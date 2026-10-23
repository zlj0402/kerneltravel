import math
import random

def get_input() -> str:
    ret_key = ''
    while not ret_key:
        ret_key = input("Please input your original key:").strip()
        if not ret_key:
            print("input err!\n")
    return ret_key

def split_32_key(s: str, n: int):
    lst = [s[i : i + n] for i in range(0, len(s), n)]
    # print(lst)
    return lst

def revert_last_half(lst: list):
    l_len = len(lst)
    for i in range(l_len //2, l_len):
        tmp = lst[i]
        lst[i] = ''.join(reversed(list(tmp)))
    # print(lst)
    return lst

def swap(lst):
    l_len = len(lst)
    for i in range(l_len // 2):
        op = l_len - i - 1
        lst[i], lst[op] = lst[op], lst[i]
    # print(lst)
    return lst

def one_action(s: str, n: int):
    slst = split_32_key(s, n)
    slst = revert_last_half(slst)
    slst = swap(slst)
    return ''.join(slst)

def actions(key: str):
    if len(key) != 32:
        print(f"key length is {len(key)}, expected 32")
    n = 2
    s = key
    while 2 * n <= len(key):
        print(f"---- {int(math.log(n, 2))} ----")
        rstr = str(random.randint(int(math.pow(10, n - 1)), int(math.pow(10, n)) - 1))
        rstr = rstr.replace(rstr[n-2], chr(int(rstr[n-2]) + ord('a')), 1)
        s = one_action((rstr + s)[:32], n)
        n = 2 * n
    return s
    
def main():
    ret_key = get_input()
    if ret_key.startswith("sk-sp-"):
        ret_key = ret_key.replace("sk-sp-", '')
    else:
        return -1

    print(actions(ret_key))


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"catched err: {e}")
