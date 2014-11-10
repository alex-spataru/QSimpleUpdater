--------------
 PREFACE
--------------

 Many websites today use the HTTPS protocol, which means that you will need SSL in order to communicate with them. If your project needs to access such a webiste (for example GitHub), you will need to carefully read the following information in order to ensure that QSimpleUpdater works with those websites (both in your machine and in the final users' machine).

This readme guide is extremely important for any developer wishing to deploy his or her applications under the Windows platform, because the application will depend on the libraries provided by QSimpleUpdater.

--------------
 LINUX      
--------------

  Make sure that you have installed the following libraries in your system:

    - lssl
    - lcrypto

--------------
 MAC OSX
--------------

  The libraries required by QSimpleUpdater are the same as Linux, however, these libraries are installed by default in most Mac OS X installations.

--------------
 WINDOWS
--------------

 QSimpleUpdater makes use of the OpenSSL-Win32 project, make sure that you deploy the following DLLs allong your application (the DLLs are provided with the source of QSimpleUpdater):

    - dependencies/OpenSSL-Win32/bin/libeay32.dll
    - dependencies/OpenSSL-Win32/bin/ssleay32.dll