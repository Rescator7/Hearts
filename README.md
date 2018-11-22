# Hearts 0.9b
The card game Hearts for QT (linux)

1. Requirements:
- Qt Creator
- QT Widget 5
- Liballegro 5

2. Install: (under ubuntu 16.04)
<pre><code>   sudo apt-get update
   sudo apt-get install git
   sudo apt-get install qtcreator
   sudo apt-get install build-essential
   sudo apt-get install qt5-default
   sudo apt-get install liballegro5-dev</code></pre>
   
   ## After that you need to download Hearts repertory
   <pre><code>git clone https://github.com/Rescator7/Hearts.git</code></pre>

   ## Start up qtcreator
   <pre><code>qtcreator</code></pre>
   
   - Open Project (Locate the directory Hearts, and open Hearts/Hearts.pro)
   - Settings file for "Hearts" from a diffrent environment. Choose: Yes.
   
   ## Maximize window (on the top bar select)
   - build -> run QMake
   - build -> build all (wait for about 5 mins to compilation process)
   - build -> run (if you want)
   
   ### After that the executable of Hearts should be located in a directory like build-Heart-Desktop-Debug/Hearts
   
3. Licenses: 
 - MIT for the software
 - Cards set: https://github.com/htdebeer/SVG-cards/blob/master/LICENSE
 - Sounds are CC 3.0 and 1.0
 - Icons: CC 1.0
<p>(read credits for more informations)</p>

![screenshoot](https://github.com/Rescator7/Hearts/blob/master/screenshot/hearts.jpg)
