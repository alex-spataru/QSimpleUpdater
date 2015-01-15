## QSimpleUpdater
[![OpenHub](https://www.openhub.net/p/qsimpleupdater/widgets/project_thin_badge.gif)](http://openhub.net/p/qsimpleupdater)

QSimpleUpdater is an implementation of an auto-updating system to be used with Qt projects. 

QSimpleUpdater is **free and open source [LGPL software](https://www.gnu.org/licenses/lgpl.html)**, which means that you can use it for both open source and proprietary applications.

## Using QSimpleUpdater
1. Copy the <code>QSimpleUpdater</code> folder in your "3rd-party" folder.
2. Include the QSimpleUpdater project include (pri) file using the include() function.
3. That's all! Check the example project as a reference for your project.

#### Running the example project
1. Navigate to the <code>Example</code> folder and open <code>example.pro</code> with [Qt Creator](http://qt-project.org/wiki/Category:Tools::QtCreator).
2. Compile the project and play with it <code>:)</code>

## Warnings 
Many websites today use the HTTPS protocol, which means that you will need SSL in order to communicate with them. If your project needs to access such a webiste (for example GitHub), you will need to carefully read the following information in order to ensure that QSimpleUpdater works with those websites (both in your machine and in the final users' machine).

This section is extremely important for any developer wishing to deploy his or her applications under the Windows platform, because the application will depend on the libraries provided by QSimpleUpdater.

### Linux
Make sure that you have installed the following libraries in your system:
+ lssl
+ lcrypto

### Mac OS X
The libraries required by QSimpleUpdater are the same as Linux, however, these libraries are installed by default in most Mac OS X installations.

### Windows
QSimpleUpdater makes use of the OpenSSL-Win32 project, make sure that have it installed and that the project knows where to find them (the default location is C:/OpenSSL-Win32) and that you deploy the following libraries with your compiled project:
+ libeay32.dll
+ ssleay32.dll

## Donate
Donate [Bitcoins](http://bitcoin.org) to the project to keep it going!
> 1BdxESMayJAengjAkjipMwfWkiqZUztyhU

## Useful Links
+ [Project website](http://alex-97.github.io/QSimpleUpdater)
+ [OpenHub Project](http://openhub.net/p/qsimpleupdater)
+ [Contact developer](mailto:alex.racotta@gmail.com)