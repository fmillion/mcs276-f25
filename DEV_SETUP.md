# Development Environment Setup

These instructions will guide you on how to set up your development environment for the assignments and projects in MCS 276.

## Windows

On Windows, the best option is to use **Windows Subsystem for Linux**. [WSL](https://learn.microsoft.com/en-us/windows/wsl/) is a compatibility layer provided by Microsoft that allows you to seamlessly use a Linux virtual machine alongside Windows without the need to install any virtual machine software, and with native support for sharing files between your PC and the Linux virtual machine. 

To install WSL, follow these steps:

1. Bring up a command prompt as *administrator*. A simple way to do this is to press `Windows+R`, type `cmd` into the Run dialog box, then press `Ctrl+Shift+Enter`. You'll be prompted to authorize the administrator command line (and will be asked to input an administrator password if you are not an admin on your machine). 
2. Enter the command `wsl --install ubuntu` and press Enter.

    The install process may take some time, and you may be asked to reboot your computer. If so, *reboot your computer before continuing*.
3. Upon rebooting (or after installation if no reboot is needed) you'll be asked to input a UNIX username and password. These can be, but *need not be*, the same as your Windows login information.

    > [!IMPORTANT]
    > Be aware of the fact that your Windows account and your Linux account are **not linked**. If you change your Windows password (or Linux password), your other password wil *not* be synchronized!
4. Once the installation finishes, you should be placed at a Linux prompt! It will look something similar to this:

        user@DESKTOP-ABCD123:~$

5. **Update the Linux installation** to the most recent base software.
    - Enter the command: `sudo apt -y update` and press `Enter`. This command downloads the latest version of the list of software components for Ubuntu. 
    - You may be asked to input your password. If so, enter the password that you set in step 3.
  
    > [!TIP]
    > The `sudo` command is a Linux command that runs the program given as an administrator (i.e. as `root`) within the Linux environment.
    >
    > The first time you use `sudo` in each Linux session, and occasionally after some time has passed, you will be asked to type in your password. The prompt is asking you to type in your *user password* - i.e. the one that you set in Step 3.

    - Once the previous command finishes, type `apt -y upgrade` and press Enter. This will take some time. If you are asked any questions during the upgrade, you may press `Enter` to accept the defaults.
    - We will learn more about these commands later in this course and in Systems 2.
6. Install the development tools suite:
    - At the Linux prompt, type `sudo apt -y install build-essential` and press `Enter`.
  
        The installation will take some time.
    - Check if the installation succeeded by trying to run the `gcc` C compiler. If you see the following:

            user@DESKTOP-1234ABC:~$ gcc
            gcc: fatal error: no input files
            compilation terminated
            user@DESKTOP-1234ABC:~$
    
        it means your build environment was installed successfully!

        If you instead see something like this:

            user@DESKTOP-1234ABC:~$ gcc
            bash: gcc: command not found
            user@DESKTOP-1234ABC:~$
        
        it means GCC was not successfully installed. You can try to exit the WSL environment (see below) and try to run `gcc` again after restarting WSL. If that also fails, try installing again with `sudo apt -y install build-essential`. 
        
If you are receiving unexpected error messages please reach out to me for assistance. Provide screenshots and/or logs as appropriate when asking for help!

### Starting and Stopping

[Windows Terminal](https://learn.microsoft.com/en-us/windows/terminal/) is the most straightforward way to access WSL. If you are on Windows 11, you already have Windows Terminal. Windows 10 users can install it from the Microsoft Store [here](https://apps.microsoft.com/detail/9n0dx20hk701?hl=en-US&gl=US)

If you have Windows Terminal installed, WSL should have added a new entry to your Profiles. If you click the `+` symbol in the tab bar, you should see "Ubuntu" as an option. At any time, you can bring up Windows Terminal, open a new Ubuntu window, and access WSL.

If you ever need to explicitly *shut down* the WSL virtual machine, you can open a Command Prompt or a PowerShell window in Windows Terminal and type:

    wsl --shutdown

This will immediately terminate any programs you might have running on Linux, so be cautious. 

> [!CAUTION]
> The WSL virtual machine retains your data, but it is not configured to run background services by default. While you can run any Linux server program in WSL as you'd expect, closing all WSL terminals will cause all WSL programs to be terminated and the WSL virtual machine to be automatically shut down after a short time period.
>
> If you need to keep WSL running, make sure to keep a terminal window open to it.

## Mac

The [Xcode Command Line Tools](https://developer.apple.com/xcode/resources/) for Mac provide a build of GCC that you can use to build C code at the command line.

1. Open a Terminal on your Mac.
2. Type the command:

        xcode-select --install

    and press Enter. Accept any license agreements presented and enter your password if necessary.
3. Try to run GCC as noted in step 6 for the Windows/WSL instructions. If GCC does not report "command not found", you are ready to go!

## Linux

If you are using Ubuntu Linux, you can simply follow the [WSL instructions](#windows) starting at step 5.

Many other distributions are also `apt`-based, so you can use the same command (`sudo apt -y install build-essential`) to install GCC. Distributions supporting this include:

* Debian
* Linux Mint
* Pop! OS
* Kali Linux
* Zorin OS

If you're on another distribution, you'll need to figure out exactly which package to install to get GCC - and how to install it. For your convenience, here are a few commands that will work on some popular distributions:

* Fedora (and some other RPM-based distros): `sudo dns groupinstall 'Development Tools'
* OpenSUSE: `sudo zypper install gcc gcc-c++`
* Arch Linux, Manjaro, EndeavourOS and other `pacman`-based distributions: `sudo pacman -Sy --needed build-essential`

# Sniff Test - Does GCC work?

Here is the quintessential "Hello World" program written in C:

```
#include <stdio.h>

int main(void) {
    printf("Hello World!\n");
}
```

To test GCC, first copy this short program exactly as given into a file called `hello.c` inside your Linux environment.

> [!HINT]
> An excellent method for editing your code - on any platform - is to use [Visual Studio Code](https://code.visualstudio.com/).
>
> On any platform, you can install VS Code, and then, within your terminal, enter `code .` to bring up VS Code with the directory you're currently in already open.
>
> You can also give a filename, such as `code hello.c` - an empty document will be opened if the file doesn't exist.
>
> This is *especially* useful on Windows with WSL, since the `code` program automatically handles connecting VS Code to your WSL virtual machine!

Once you've copied the program into a C source file, run this command at your terminal:

    gcc -o hello hello.c

If you get *no output*, the compile should have succeeded! Run your program by typing:

    ./hello

You should see something like:

    user@DESKTOP-1234ABC:~$ ./hello
    Hello World
    user@DESKTOP-1234ABC:~$

If so, congratulations - your system is set up to build and run C code!
