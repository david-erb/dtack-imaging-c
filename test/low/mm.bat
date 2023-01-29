bcc32 /WM /c /K /d /WC /v /N /O1 /k /c tt.c
tlink32 -v -Tpe -ap -c c0x32 tt.obj, tt.exe, tt.map, import32 cw32
