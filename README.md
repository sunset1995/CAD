# How to work with git
1. git clone git@github.com:sunset1995/CAD.git yourDir
2. cd yourDir

we may use

1. git pull
  * pull the latest version of the project.
    * no confilct
        There is no diffirent OR the diffirent between your local and remote is done by you
    * confilct
        It will alert you which file cause a confilct. It will insert some string in that file, tell you where is the confilct. simply edit that file by your hand and so that you can push
2. git push
    * Push your local version to update remote repo. Make sure you pull everytime before push. If confilct github won't let you push

3. git status
    * Show all file you have edited

4. git add .
    * Add all file you have edited ready to push

5. git commit -m "briefly tell others what you have done"

*all the task flow like this
    *clone
    *git pull -> edit -> git add . -> git commim -m "..." -> git pull -> git push

# How to compile
```
  g++ -O2 -w -std=c++11 -c ClassDefine.cpp
  g++ -O2 -w -std=c++11 -c WindowSet.cpp
  g++ -O2 -w -std=c++11 -c ScanLine.cpp
  g++ -O2 -w -std=c++11 -c DisjointSet.cpp
  g++ -O2 -w -std=c++11 -c IOInterface.cpp
  g++ -O2 -w -std=c++11 -c main.cpp
  g++ -O2 -w -std=c++11 -o test(.exe) ClassDefine.o WindowSet.o ScanLine.o DisjointSet.o IOInterface.o main.o
```
