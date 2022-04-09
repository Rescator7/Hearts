# Hearts 1.8.10 (Linux, C++)
The card game Hearts for QT 5 or 6, and liballegro 5 (for sounds support)

<p>You can download Hearts as a snap package from:</p>

<a href="https://snapcraft.io/hearts">
  <img alt="Get it from the Snap Store" src="https://snapcraft.io/static/images/badges/en/snap-store-black.svg"/></a>
<br>
<p>Alternatively, you can download an AppImage from https://www.linux-apps.com/p/1275760/ in the files section.</p>
<p>Or see below "How to build the source code" on your computer.</p>

## Game Features:
- Support online and offline game play.
- Support 3 languages. (English, French, Russian). 
- Support 5 differents deck: Standard, Nicu (white), English, Russian, Tigullio (modern).
- 5 background images theme: Universe, Ocean, Mt. Fuji, Everest, Desert.
- Helps [Rules, Online, Settings, Credits]
- Sounds.
- Easy card selection. (offline only)
- Detect TRAM "The rest are mine" (offline only).

## Game Variants [Turn on/off]:
- Queen of spade breaks heart.
- Omnibus.   (Jack Diamond -10 pts).
- No Tricks. (Bonus -5 pts).
- New Moon. <p>(Allow human player who shoot the moon to choose between add 26 pts to the opponents, or 
             substract 26 pts to his own score).</p>
- Perfect 100. (A player who get the score of 100 pts have his score reduce to 50 pts).
- No Draw.   (Disable multiple winners).

## Offline Options:
- Undo. 
- Animated play.
- Three pre-defined game speed: Slow, Normal, Fast. 
- A Customized game speed option (for expert only).
- Autosave/Autoreload of the unfinished game.
- Cheat mode. (Reveal your opponents's cards).
- Cards history.
- Cards played list.
- Complete game statistics.
<p>and more...</p>
<p>Note: Undo is only available when it's your turn to play. You must first have played a card before.
  No undo available from a saved game. The undo will be purged every round or if you connect to a server. 
  Not available online. Only 1 undo at a time is possible. e.g: No multiple undos in a row.</p>

# How to build the source code.

## 1. Requirements:
- QT Widget 5
- QT SVG 5
- Liballegro 5

<p>NOTE: Version 1.8 and above require at lease 4G of ram to build.</p>
<pre><code>g++ -c -pipe -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DONLINE_PLAY -DUSE_LIBALLEGRO5 -DFULL_SCREEN -DDEBUG -DQT_NO_DEBUG -DQT_SVG_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -I. -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtSvg -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui -isystem /usr/include/x86_64-linux-gnu/qt5/QtNetwork -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o qrc_resources.o qrc_resources.cpp
g++: fatal error: Killed signal terminated program cc1plus
compilation terminated.
make: *** [Makefile:902: qrc_resources.o] Error 1</code></pre>

<p>If you encounter this message. It means the compiler is running out of Memory and can't build the application. Try to free some memory.</p>

## 2. Install requirements: (under ubuntu 16.04)
<pre><code> sudo apt-get update
 sudo apt-get install git
 sudo apt-get install build-essential
 sudo apt-get install qt5-default
 sudo apt-get install liballegro5-dev
 sudo apt-get install libqt5svg5-dev</code></pre>
   
<p>NOTE: libqt5svg5-dev is required since version 1.7</p>

## 3. Download the source code
<pre><code> git clone https://github.com/Rescator7/Hearts.git</code></pre>
   
### 3a. Build the code
<pre><code> cd Hearts
 qmake Hearts.pro
 make</code></pre>
 
### 3b. Install / Execute Hearts
<p>Please note: There is no "make install" as the binary "Hearts" already contains all the components: icons, sounds, images, etc. It can be run by (from the build directory):</p> 
<code> ./Hearts</code>
<p></p>
<p>It could be copied to the Desktop (run by double-click Hearts)</p>
<code>cp ./Hearts ~/Desktop</code>
<p></p>
<p>Or copied to a "common" binaries's path such as:</p>
<pre><code>cp ./Hearts /usr/local/bin</code>
Hearts</code></pre>
   
## 4. Licenses: 
 - MIT for the software (source code)
 - Standard cards deck: https://github.com/htdebeer/SVG-cards/blob/master/LICENSE
 - Nicu (white) cards deck: https://creativecommons.org/licenses/publicdomain/
 - Sounds are CC 3.0 and 1.0
 - Background images are CC 3.0 and 1.0
 - Icons: (read credits)
 - READ CREDITS for all the licenses details: https://github.com/Rescator7/Hearts/blob/master/Credits

![screenshoot](https://github.com/Rescator7/Hearts/blob/master/screenshot/SCR1-hearts-1.8.jpg)
