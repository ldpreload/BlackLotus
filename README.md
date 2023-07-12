# BlackLotus
BlackLotus is an innovative UEFI Bootkit designed specifically for Windows. It incorporates a built-in Secure Boot bypass and Ring0/Kernel protection to safeguard against any attempts at removal. This software serves the purpose of functioning as an HTTP Loader. Thanks to its robust persistence, there is no necessity for frequent updates of the Agent with new encryption methods. Once deployed, traditional antivirus software will be incapable of scanning and eliminating it. The software comprises two primary components: the Agent, which is installed on the targeted device, and the Web Interface, utilized by administrators to manage the bots. In this context, a bot refers to a device equipped with the installed Agent.

**FYI**: This version of BlackLotus (v2) has removed baton drop, and replaced the original version SHIM loaders with bootlicker. UEFI loading, infection and post-exploitation persistence are all the same.

## General
- Written in C and x86asm
- Utilizes on Windows API, NTAPI, EFIAPI (NO 3rd party libraries used),
- NO CRT (C Runtime Library).
- Compiled binary including the user-mode loader is only 80kb in size
- Uses secure HTTPS C2 communication by using RSA and AES encryption
- Dynamic configuration


## Features
- HVCI bypass
- UAC bypass
- Secure Boot bypass
- BitLocker boot sequence bypass
- Windows Defender bypass (patch Windows Defender drivers in memory, and prevent Windows Defender usermode engine from scanning/uploading files)
- Dynamic hashed API calls (hell's gate)
- x86<=>x64 process injection
- API Hooking engine
- Anti-Hooking engine (for disabling, bypassing, and controlling EDRs)
- Modular plugin system


Setup by modifying the config.c file by including your C2s hostname or IP address.
After that compliation should be easy, just keep the included settings in the Visual Studio solution.

## Default Panel Credentials:

- **user**: yukari
- **password**: default

## References

* Welivesecurity: https://www.welivesecurity.com/2023/03/01/blacklotus-uefi-bootkit-myth-confirmed

* Binarly: https://www.binarly.io/posts/The_Untold_Story_of_the_BlackLotus_UEFI_Bootkit/index.html

* NSA Mitigation Guide: https://www.nsa.gov/Press-Room/Press-Releases-Statements/Press-Release-View/Article/3435305/nsa-releases-guide-to-mitigate-blacklotus-threat

* TheHackerNews: https://thehackernews.com/2023/03/blacklotus-becomes-first-uefi-bootkit.html

* Bootlicker: https://github.com/realoriginal/bootlicker
