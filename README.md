# Mpnotify-steal-password
Loads a DLL to Mpnotify and send the password to hacker's server. It is a modified version of nppspy where the malicious DLL send the password to the hacker's server.

# Compile
```
gcc -shared -o NPPSpy.dll file.c
```

# How it works
Source: https://www.huntress.com/blog/cleartext-shenanigans-gifting-user-passwords-to-adversaries-with-nppspy

The conversation between Winlogon and Local Security Authority Subsystem Service (LSASS) is most relevant for our instance. Winlogon is both the graphical user interface that we use to put our credentials in, as well as the conversational partner with LSASS for letting you sign in. 

It’s more of a challenge to mess with LSASS to try and gather credentials, and so the NPPSPY technique takes the path of least resistance by focusing on Winlogon. 

When you give your password to Winlogon, it opens up an (RPC) channel to a mpnotify.exe and sends it over the password. Mpnotify then goes and tells some DLLs what’s up with this credential. 

NPPSPY comes alive here. Mpnotify is maliciously told about a new adversarial network provider to consider. This network provider is attacker-controlled and comes with a backdoored DLL the adversary has created. This slippery DLL simply listens for this clear text credential exchange from winlogon down to mpnotify and then saves this clear text credential exchange. 

![image](https://github.com/diego-tella/NpNotify-steal-password/assets/70545257/62a6c12e-3211-4872-a27c-07bb2c89a72d)


# Credits
Credits to <b>gtworek</b>. Original code here: https://github.com/gtworek/PSBits/tree/master/PasswordStealing/NPPSpy

https://www.youtube.com/watch?v=ggY3srD9dYs
