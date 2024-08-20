.intel_syntax noprefix
.global _main

_main:
# ; ------------------------ prolong and local vars ------------------------ ;

push    ebp                         # store ebp
mov     ebp, esp
sub	    esp, 128

mov     DWORD PTR [ebp - 4], 0      # ORE
mov     DWORD PTR [ebp - 8], 0      # GRAIN
mov     DWORD PTR [ebp - 12], 0     # WOOL
mov     DWORD PTR [ebp - 16], 0     # BRICK
mov     DWORD PTR [ebp - 20], 0     # WOOD

# ; ------------------------ get GetProcAddress addr ------------------------ ;

push    0x0       		# pushing null
push    0x41797261		# pushing A,y,r,a
push    0x7262694c		# pushing r,b,i,L
push    0x64616f4c		# pushing d,a,o,L
push    esp             # push pointer for "LoadLibraryA"

call    FindFunction    # call FindFunction("LoadLibraryA")
add     esp, 0x14       # clear stack

push    0x00006c6c		# pushing null,l,l
push    0x642e7472		# pushing d,#,t,r
push    0x6376736d		# pushing c,v,s,m
push    esp

call    eax             # call LoadLibrary("msvcrt.dll")
add     esp, 0x10       # clear stack (note arguments are cleared already)

push    eax             # store module handle for msvcrt
push    0x00007373		# pushing null,s,s
push    0x65726464		# pushing e,r,d,d
push    0x41636f72		# pushing A,c,o,r
push    0x50746547		# pushing P,t,e,G
push    esp             # push pointer for "GetProcAddress"

call    FindFunction    # call FindFunction("GetProcAddress")
add     esp, 0x14       # clear stack
pop     ebx             # restore module handle for msvcrt

mov     DWORD PTR [ebp - 48], eax       # GetProcAddress addr

# ; ------------------------ get addrs for scanf ------------------------ ;

mov     eax, DWORD PTR [ebp - 48]       # GetProcAddress addr

push    0x00000066      # pushing null,f
push    0x6e616373      # pushing n,a,c,s
push    esp             # push pointer for "scanf"
push    ebx             # push module handle for msvcrt

call    eax             # call GetProcAddress(msvcrt, "scanf")
add     esp, 0x10       # clear stack (note arguments are cleared already)

mov     DWORD PTR [ebp - 56], eax       # scanf addr

# ; ------------------------ get addrs for printf ------------------------ ;

mov     eax, DWORD PTR [ebp - 48]       # GetProcAddress addr

push    0x00006674		# pushing null,f,t
push    0x6e697270		# pushing n,i,r,p
push    esp             # push pointer for "printf"
push    ebx             # push module handle for msvcrt

call    eax             # call GetProcAddress(msvcrt, "printf")
add     esp, 0x10       # clear stack (note arguments are cleared already)

mov     DWORD PTR [ebp - 64], eax       # printf addr

# ; ------------------------ get addrs for strcmp ------------------------ ;

mov     eax, DWORD PTR [ebp - 48]       # GetProcAddress addr

push    0x0000706d      # pushing null,p,m
push    0x63727473      # pushing c,r,t,s
push    esp             # push pointer for "strcmp"
push    ebx             # push module handle for msvcrt

call    eax             # call GetProcAddress(msvcrt, "strcmp")
add     esp, 0x10       # clear stack (note arguments are cleared already)

mov     DWORD PTR [ebp - 72], eax       # strcmp addr

# ; ------------------------ get addrs for strlen ------------------------ ;

mov     eax, DWORD PTR [ebp - 48]       # GetProcAddress addr

push    0x00006e65      # pushing null,n,e
push    0x6c727473      # pushing l,r,t,s
push    esp             # push pointer for "strlen"
push    ebx             # push module handle for msvcrt

call    eax             # call GetProcAddress(msvcrt, "strlen")
add     esp, 0x10       # clear stack (note arguments are cleared already)

mov     DWORD PTR [ebp - 80], eax       # strlen addr

# ; ------------------------ main while loop ------------------------ ;

_while_header:

mov     eax, DWORD PTR [ebp - 56]       # scanf addr

push    0x00007325          # pushing null,s,%
lea	    edx, [ebp - 40]
push    edx                 # push pointer for string
lea	    edx, [esp + 4]
push    edx                 # push pointer for "%s"

call    eax                 # call scanf("%s", str)
add     esp, 0x0c           # clear stack (note arguments are cleared already)

test	eax, eax
jle	    _while_end

# ; ------------------------ main while body ------------------------ ;

_while_body:

# ; ------------------------ str[linelen - 1] =? '\n' ------------------------ ;

lea	    edx, [ebp - 40]                     # input from scanf
push    edx                                 # push pointer for string

mov     eax, DWORD PTR [ebp - 80]           # strlen addr
call    eax                                 # call strlen(str)
add     esp, 0x04                           # clear stack

sub	    eax, 1                              # linelen - 1
movzx   eax, BYTE PTR [ebp - 40 + eax]      # str[linelen - 1]
cmp     al, 10
jne     _road
mov     BYTE PTR [ebp - 40 + eax], 0

# ; ------------------------ road case ------------------------ ;

_road:
push    0x00000000          # pushing null
push    0x64616f72          # pushing d,a,o,r
push    esp                 # push pointer for "road"
lea	    edx, [ebp - 40]     # input from scanf
push    edx                 # push pointer for str

mov     eax, DWORD PTR [ebp - 72]           # strcmp addr
call    eax                                 # call strcmp(str, "road")
add     esp, 0x10

cmp     eax, 0              
jne     _city               # if strcmp(str, "road") != 0 goto city

add     DWORD PTR [ebp - 16], 1     # BRICK
add     DWORD PTR [ebp - 20], 1     # WOOD
jmp     _while_header

_city:
push    0x00000000          # pushing null
push    0x79746963          # pushing y,t,i,c
push    esp                 # push pointer for "city"
lea	    edx, [ebp - 40]     # input from scanf
push    edx                 # push pointer for str

mov     eax, DWORD PTR [ebp - 72]           # strcmp addr
call    eax                                 # call strcmp(str, "city")
add     esp, 0x10

cmp     eax, 0              
jne     _settlement         # if strcmp(str, "city") != 0 goto settlement

add     DWORD PTR [ebp - 4], 3      # ORE
add     DWORD PTR [ebp - 8], 2      # GRAIN
jmp     _while_header

_settlement:
push    0x0000746e          # pushing null,t,n
push    0x656d656c          # pushing e,m,e,l
push    0x74746573          # pushing t,t,e,s
push    esp                 # push pointer for "settlement"
lea	    edx, [ebp - 40]     # input from scanf
push    edx                 # push pointer for str

mov     eax, DWORD PTR [ebp - 72]           # strcmp addr
call    eax                                 # call strcmp(str, "settlement")
add     esp, 0x14

cmp     eax, 0              
jne     _development        # if strcmp(str, "city") != 0 goto development

add     DWORD PTR [ebp - 8], 1      # GRAIN
add     DWORD PTR [ebp - 12], 1     # WOOL
add     DWORD PTR [ebp - 16], 1     # BRICK
add     DWORD PTR [ebp - 20], 1     # WOOD
jmp     _while_header

_development:
push    0x00746e65          # pushing null,t,n,e
push    0x6d706f6c          # pushing m,p,o,l
push    0x65766564          # pushing e,v,e,d
push    esp                 # push pointer for "development"
lea	    edx, [ebp - 40]     # input from scanf
push    edx                 # push pointer for str

mov     eax, DWORD PTR [ebp - 72]           # strcmp addr
call    eax                                 # call strcmp(str, "development")
add     esp, 0x14

cmp     eax, 0              
jne     _while_header        # if strcmp(str, "development") != 0 goto _while_header

add     DWORD PTR [ebp - 4], 1      # ORE
add     DWORD PTR [ebp - 8], 1      # GRAIN
add     DWORD PTR [ebp - 12], 1     # WOOL
jmp     _while_header

_while_end:

# ; ------------------------ print results ------------------------ ;

mov     DWORD PTR [ebp - 84], 0
jmp     .L2

.L5:
    mov     eax, DWORD PTR [ebp - 84]               # counter
    mov     eax, DWORD PTR [ebp - 20 + eax * 4]     # next output
    cmp     eax, 9
    jg      .L3

    push    0x00000030      # pushing null,0
    push    esp             # push pointer for "0"

    mov     eax, DWORD PTR [ebp - 64]               # printf addr
    call    eax             # call printf("0")
    add     esp, 0x08       # clear stack

.L3:
    mov     eax, DWORD PTR [ebp - 84]               # counter
    mov     eax, DWORD PTR [ebp - 20 + eax * 4]     # next output

    push    0x00006425      # pushing null,d,%
    push    eax             # pushing out[i]
    lea	    edx, [esp + 4]
    push    edx             # push pointer for "%d"

    mov     eax, DWORD PTR [ebp - 64]               # printf addr
    call    eax             # call printf("%d", out[i])
    add     esp, 0x0c       # clear stack

    cmp     DWORD PTR [ebp - 84], 3
    jg      .L4

    push    0x00000020      # pushing null,SPACE
    push    esp             # push pointer for " "

    mov     eax, DWORD PTR [ebp - 64]               # printf addr
    call    eax             # call printf(" ")
    add     esp, 0x08       # clear stack

.L4:
    add     DWORD PTR [ebp - 84], 1         # counter

.L2:
    cmp     DWORD PTR [ebp - 84], 4         # counter
    jle     .L5


mov     eax, 0          # set exit code to 0
leave
ret                     # return

FindFunction:       # placeholder for search_kernel32
