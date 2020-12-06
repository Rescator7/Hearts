# Hearts 1.6.1 (Linux)
The card game Hearts for QT 5.x and liballegro 5 (for sounds support)

## Game Features:
- Support online and offline game play.
- Support 3 languages. (English, French, Russian). 
- Support 3 differents deck: Standard, English, Russian.
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

### Offline Options:
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

## 1. Requirements:
- Qt Creator (suggested)
- QT Widget 5
- Liballegro 5

## 2. Install: (under ubuntu 16.04)
<pre><code> sudo apt-get update
 sudo apt-get install git
 sudo apt-get install qtcreator 
 sudo apt-get install build-essential
 sudo apt-get install qt5-default
 sudo apt-get install liballegro5-dev</code></pre>
   
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
<p>(read credits for more informations)</p>

![screenshoot](https://github.com/Rescator7/Hearts/blob/master/screenshot/SCR1-hearts-1.6.jpg)
