# Project One by Ahmed Abdul Rahman
### Annoying Mouse Room DELUXE is a Karel-like game (for windows only) made using the [Raylib](https://github.com/raysan5/raylib) library and [Sol2](https://github.com/ThePhD/sol2) where you use pre-set commands via a text file to guide Maus to the goal!

## How the gameplay works
In the game directory, there's a folder called `workers`, and inside that folder, there's a text file called `MausWorker.txt` By looking at the tutorial, you can be able to input your commands that way. 
![KarelGame_Ud5OZitls8](https://github.com/user-attachments/assets/051e8737-9a48-4f95-a660-62104a40882c)

# Known Bugs
* ```lua while(true){ }``` hasn't been tested properly and may break the game.
* Opening the text file from the button (by uncommenting [line 214 in main.cpp](https://github.com/AhmedRamen/KarelGame/blob/05ecf074efd8e620549dcfe766664bc5e974b0be/src/main.cpp#L214)) freezes all operations until you close it. (I haven't implemented a proper way to open up the folder like how Godot does it)
* In the release build, closing the game actually "crashes" the game.
