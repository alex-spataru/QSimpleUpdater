## QSimpleUpdater

QSimpleUpdater is an implementation of an auto-updating system to be used with Qt projects. 

QSimpleUpdater is **free and open source [LGPL software](https://www.gnu.org/licenses/lgpl.html)**, which means that you can use it for both open source and propietary applications.

## Using QSimpleUpdater

#### 1. Import QSimpleUpdater to your project

1. Copy the <code>QSimpleUpdater</code> folder in your "3rd-party" folder.
3. Include the QSimpleUpdater project include (pri) file using the include() function.
5. That's all! Check the example project as a reference.

#### 2. Include QSimpleUpdater in your header file(s):

<div style="background: #ffffff; overflow:auto;width:auto;border:solid gray;border-width:.1em .1em .1em .8em;padding:.2em .6em;"><pre style="margin: 0; line-height: 125%"><span style="color: #557799">#include &lt;QSimpleUpdater&gt;</span></pre></div>

#### 3. Declare a new instance of QSimpleUpdater in your header file (preferably as a private object).

<div style="background: #ffffff; overflow:auto;width:auto;border:solid gray;border-width:.1em .1em .1em .8em;padding:.2em .6em;"><pre style="margin: 0; line-height: 125%"><span style="color: #557799">QSimpleUpdater *updater;</span></pre></div>

#### 4. Initialize and configure the updater when your class is created:

<div style="background: #ffffff; overflow:auto;width:auto;border:solid gray;border-width:.1em .1em .1em .8em;padding:.2em .6em;"><pre style="margin: 0; line-height: 125%"> MyClass<span style="color: #333333">::</span>MyClass() { 
    
    <span style="color: #888888">// Initialize the updater</span>
    updater <span style="color: #333333">=</span> <span style="color: #008800; font-weight: bold">new</span> QSimpleUpdater(<span style="color: #008800; font-weight: bold">this</span>);

    <span style="color: #888888">// Define our application version....</span>
    <span style="color: #888888">// The string must contain the same number</span>
    <span style="color: #888888">// of dots as the one that we will download</span>
    QString app_version <span style="color: #333333">=</span> <span style="background-color: #fff0f0">&quot;1.2.3&quot;</span>
              
    <span style="color: #888888">// This string will help us to specify which file</span>
    <span style="color: #888888">// should we download when a new version of the app</span>
    <span style="color: #888888">// is detected.</span>
    QString download_url;
              
    <span style="color: #888888">// The following code will help us to define from where</span>
    <span style="color: #888888">// we should download the binary installation file </span>
    <span style="color: #888888">// in the case that the updater detects a newer version</span>
    <span style="color: #888888">// of your app.</span>
              
    <span style="color: #888888">// Download the DMG file of your app </span>
    <span style="color: #FF0000; background-color: #FFAAAA">#</span>ifdef Q_OS_MAC
        download_url <span style="color: #333333">=</span> <span style="background-color: #fff0f0">&quot;http://myapp.com/downloads/latest.dmg&quot;</span>;
    <span style="color: #FF0000; background-color: #FFAAAA">#</span>endif
              
    <span style="color: #888888">// Download the EXE setup for your app</span>
    <span style="color: #FF0000; background-color: #FFAAAA">#</span>ifdef Q_OS_WIN
        download_url <span style="color: #333333">=</span> <span style="background-color: #fff0f0">&quot;http://myapp.com/downloads/latest.exe&quot;</span>;
    <span style="color: #FF0000; background-color: #FFAAAA">#</span>endif
              
    <span style="color: #888888">// Download a *.tar.gz file for your app</span>
    <span style="color: #FF0000; background-color: #FFAAAA">#</span>ifdef Q_OS_LINUX
        download_url <span style="color: #333333">=</span> <span style="background-color: #fff0f0">&quot;http://myapp.com/downloads/latest.tar.gz&quot;</span>;
    <span style="color: #FF0000; background-color: #FFAAAA">#</span>endif
             
    <span style="color: #888888">// Version of the installed application (in this case, 1.2.3)</span>
    <span style="color: #888888">// The parameter must be a QString.... </span>
    updater<span style="color: #333333">-&gt;</span>setApplicationVersion(app_version);
              
    <span style="color: #888888">// Tell the updater from where we should download</span>
    <span style="color: #888888">// the installer of our application</span>
    updater<span style="color: #333333">-&gt;</span>setDownloadUrl(QUrl(download_url));
              
    <span style="color: #888888">// The following text file should only contain the </span>
    <span style="color: #888888">// latest application version, for example, 1.2.3 or 1.2.4</span>
    updater<span style="color: #333333">-&gt;</span>setReferenceUrl(QUrl(<span style="background-color: #fff0f0">&quot;http://myapp.com/latest.txt&quot;</span>));
              
    <span style="color: #888888">// Tell the updater where to download the changelog...</span>
    updater<span style="color: #333333">-&gt;</span>setChangelogUrl(QUrl(<span style="background-color: #fff0f0">&quot;http://myapp.com/changelog.txt&quot;</span>));
              
    <span style="color: #888888">// Check for updates....</span>
    updater<span style="color: #333333">-&gt;</span>checkForUpdates();
              
    <span style="color: #888888">// Finally, do something when the updater finds a new version</span>
    <span style="color: #888888">// of your app.</span>
    connect(updater, SIGNAL(updateAvailable()), 
            <span style="color: #008800; font-weight: bold">this</span>, SLOT(onUpdateAvailable()));

}
</pre></div>

#### 5. Define what your application should do when the updater finds a new version of your application. For example:

<div style="background: #ffffff; overflow:auto;width:auto;border:solid gray;border-width:.1em .1em .1em .8em;padding:.2em .6em;"><pre style="margin: 0; line-height: 125%">MyClass<span style="color: #333333">::</span>onUpdateAvailable() {
    qDebug() <span style="color: #333333">&lt;&lt;</span> <span style="background-color: #fff0f0">&quot;A new version of myApp is available!&quot;</span>;
    qDebug() <span style="color: #333333">&lt;&lt;</span> <span style="background-color: #fff0f0">&quot;The latest version is:&quot;</span> 
             <span style="color: #333333">&lt;&lt;</span> updater<span style="color: #333333">-&gt;</span>latestVersion();
    qDebug() <span style="color: #333333">&lt;&lt;</span> <span style="background-color: #fff0f0">&quot;The change log of the new version is:</span><span style="color: #666666; font-weight: bold; background-color: #fff0f0">\n</span><span style="background-color: #fff0f0">&quot;</span> 
             <span style="color: #333333">&lt;&lt;</span> updater<span style="color: #333333">-&gt;</span>changeLog();
            
    <span style="color: #333399; font-weight: bold">char</span> type;
    <span style="color: #008800; font-weight: bold">while</span> (<span style="color: #007020">true</span>) {
        cout <span style="color: #333333">&lt;&lt;</span> <span style="background-color: #fff0f0">&quot;Download the latest version [y/n]&quot;</span> <span style="color: #333333">&lt;&lt;</span> endl;
        cin <span style="color: #333333">&gt;&gt;</span> type;

        <span style="color: #008800; font-weight: bold">if</span> ((type <span style="color: #333333">==</span> <span style="color: #0044DD">&#39;y&#39;</span>) <span style="color: #333333">||</span> (type <span style="color: #333333">==</span> <span style="color: #0044DD">&#39;n&#39;</span>)) {
            <span style="color: #008800; font-weight: bold">break</span>;
        }
    }
            
    <span style="color: #008800; font-weight: bold">if</span> (type <span style="color: #333333">==</span> <span style="color: #0044DD">&#39;y&#39;</span>) {
        updater<span style="color: #333333">-&gt;</span>downloadLatestVersion();
    }
}
</pre></div>

#### Notes


## Running the example project

1. Navigate to the <code>Example</code> folder and open <code>example.pro</code> with [Qt Creator](http://qt-project.org/wiki/Category:Tools::QtCreator).
2. Compile the project and play with it <code>:)</code>


## Useful Links

+ [Project website](http://qsimpleupdater.sf.net)
+ [SourceForge Project](http://sf.net/p/qsimpleupdater)
+ [OpenHub Project](http://openhub.net/p/qsimpleupdater)
+ [Contact developer](mailto:alex.racotta@gmail.com)

## Donate

Donate [Bitcoins](http://bitcoin.org) to the project to keep it going!

> 1BdxESMayJAengjAkjipMwfWkiqZUztyhU






 