# Hearts 1.8.4 (Linux, C++)
The card game Hearts for QT 5 and liballegro 5 (for sounds support)

<p>You can download Hearts as a snap package from:</p>

<a href="https://snapcraft.io/hearts">
  <img alt="Get it from the Snap Store" src="https://snapcraft.io/static/images/badges/en/snap-store-black.svg"/></a>

## Game Features:
- Support online and offline game play.
- Support 3 languages. (English, French, Russian). 
- Support 4 differents deck: Standard, Nicu (white), English, Russian.
- 5 background images theme: Universe, Ocean, Mt. Fuji, Everest, Desert.
- Helps [Rules, Online, Settings, Credits]
- Sounds.
- Easy cards selection.
- Detect TRAM (The rest are mine).

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
- Three game speed: Slow, Normal, Fast.
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
- Qt Creator (suggested)
- QT Widget 5
- QT SVG 5
- Liballegro 5

<p>NOTE: Version 1.8 and above require at lease 4G of ram to build.</p>
<pre><code>g++ -c -pipe -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DONLINE_PLAY -DUSE_LIBALLEGRO5 -DFULL_SCREEN -DDEBUG -DQT_NO_DEBUG -DQT_SVG_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -I. -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtSvg -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui -isystem /usr/include/x86_64-linux-gnu/qt5/QtNetwork -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o qrc_resources.o qrc_resources.cpp
g++: fatal error: Killed signal terminated program cc1plus
compilation terminated.
make: *** [Makefile:902: qrc_resources.o] Error 1</code></pre>

<p>If you encounter this message. It means the compiler is running out of Memory and can't build the application. Try to free some memory.</p>

## 2. Install: (under ubuntu 16.04)
<pre><code> sudo apt-get update
 sudo apt-get install git
 sudo apt-get install qtcreator 
 sudo apt-get install build-essential
 sudo apt-get install qt5-default
 sudo apt-get install liballegro5-dev
 sudo apt-get install libqt5svg5-dev</code></pre>
   
<p>NOTE: libqt5svg5-dev is required since version 1.7</p>

<p>After that you need to download Hearts repertory</p>
<pre><code> git clone https://github.com/Rescator7/Hearts.git</code></pre>
   
### Option #1: (without QtCreator)
<pre><code> cd Hearts
 qmake Hearts.pro
 make</code></pre>
   
### Option #2: 
   <p>Start up qtcreator</p>
   <pre><code>qtcreator</code></pre>
   
   - Open Project (Locate the directory Hearts, and open Hearts/Hearts.pro)
   - Settings file for "Hearts" from a diffrent environment. Choose: Yes.
   
   <p>Maximize window (on the top bar select)</p>
   
   - build -> run QMake
   - build -> build all (wait for about 5 mins to compilation process)
   - build -> run (if you want)
   
   <p>After that the executable of Hearts should be located in a directory like build-Heart-Desktop-Debug/Hearts</p>
   
## 3. Licenses: 
 - MIT for the software
 - Cards set: https://github.com/htdebeer/SVG-cards/blob/master/LICENSE
 - Sounds are CC 3.0 and 1.0
 - Icons: (read credits)
 - read credits for all the licenses: https://github.com/Rescator7/Hearts/blob/master/Credits

![screenshoot](https://github.com/Rescator7/Hearts/blob/master/screenshot/SCR1-hearts-1.8.jpg)
